int DefaultCertValidator::initializeSslContexts(std::vector<SSL_CTX*> contexts,
                                                bool provides_certificates) {

  int verify_mode = SSL_VERIFY_NONE;
  int verify_mode_validation_context = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;

  if (config_ != nullptr) {
    envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext::
        TrustChainVerification verification = config_->trustChainVerification();
    if (verification == envoy::extensions::transport_sockets::tls::v3::
                            CertificateValidationContext::ACCEPT_UNTRUSTED) {
      verify_mode = SSL_VERIFY_PEER; // Ensure client-certs will be requested even if we have
                                     // nothing to verify against
      verify_mode_validation_context = SSL_VERIFY_PEER;
    }
  }

  if (config_ != nullptr && !config_->caCert().empty() && !provides_certificates) {
    ca_file_path_ = config_->caCertPath();
    bssl::UniquePtr<BIO> bio(
        BIO_new_mem_buf(const_cast<char*>(config_->caCert().data()), config_->caCert().size()));
    RELEASE_ASSERT(bio != nullptr, "");
    // Based on BoringSSL's X509_load_cert_crl_file().
    bssl::UniquePtr<STACK_OF(X509_INFO)> list(
        PEM_X509_INFO_read_bio(bio.get(), nullptr, nullptr, nullptr));
    if (list == nullptr) {
      throw EnvoyException(
          absl::StrCat("Failed to load trusted CA certificates from ", config_->caCertPath()));
    }

    for (auto& ctx : contexts) {
      X509_STORE* store = SSL_CTX_get_cert_store(ctx);
      bool has_crl = false;
      for (const X509_INFO* item : list.get()) {
        if (item->x509) {
          X509_STORE_add_cert(store, item->x509);
          if (ca_cert_ == nullptr) {
            X509_up_ref(item->x509);
            ca_cert_.reset(item->x509);
          }
        }
        if (item->crl) {
          X509_STORE_add_crl(store, item->crl);
          has_crl = true;
        }
      }
      if (ca_cert_ == nullptr) {
        throw EnvoyException(
            absl::StrCat("Failed to load trusted CA certificates from ", config_->caCertPath()));
      }
      if (has_crl) {
        X509_STORE_set_flags(store, config_->onlyVerifyLeafCertificateCrl()
                                        ? X509_V_FLAG_CRL_CHECK
                                        : X509_V_FLAG_CRL_CHECK | X509_V_FLAG_CRL_CHECK_ALL);
      }
      verify_mode = SSL_VERIFY_PEER;
      verify_trusted_ca_ = true;

      // NOTE: We're using SSL_CTX_set_cert_verify_callback() instead of X509_verify_cert()
      // directly. However, our new callback is still calling X509_verify_cert() under
      // the hood. Therefore, to ignore cert expiration, we need to set the callback
      // for X509_verify_cert to ignore that error.
      if (config_->allowExpiredCertificate()) {
        X509_STORE_set_verify_cb(store, CertValidatorUtil::ignoreCertificateExpirationCallback);
      }
    }
  }

  if (config_ != nullptr && !config_->certificateRevocationList().empty()) {
    bssl::UniquePtr<BIO> bio(
        BIO_new_mem_buf(const_cast<char*>(config_->certificateRevocationList().data()),
                        config_->certificateRevocationList().size()));
    RELEASE_ASSERT(bio != nullptr, "");

    // Based on BoringSSL's X509_load_cert_crl_file().
    bssl::UniquePtr<STACK_OF(X509_INFO)> list(
        PEM_X509_INFO_read_bio(bio.get(), nullptr, nullptr, nullptr));
    if (list == nullptr) {
      throw EnvoyException(
          absl::StrCat("Failed to load CRL from ", config_->certificateRevocationListPath()));
    }

    for (auto& ctx : contexts) {
      X509_STORE* store = SSL_CTX_get_cert_store(ctx);
      for (const X509_INFO* item : list.get()) {
        if (item->crl) {
          X509_STORE_add_crl(store, item->crl);
        }
      }
      X509_STORE_set_flags(store, config_->onlyVerifyLeafCertificateCrl()
                                      ? X509_V_FLAG_CRL_CHECK
                                      : X509_V_FLAG_CRL_CHECK | X509_V_FLAG_CRL_CHECK_ALL);
    }
  }

  const Envoy::Ssl::CertificateValidationContextConfig* cert_validation_config = config_;
  if (cert_validation_config != nullptr) {
    if (!cert_validation_config->subjectAltNameMatchers().empty()) {
      for (const envoy::extensions::transport_sockets::tls::v3::SubjectAltNameMatcher& matcher :
           cert_validation_config->subjectAltNameMatchers()) {
        subject_alt_name_matchers_.emplace_back(createStringSanMatcher(matcher));
      }
      verify_mode = verify_mode_validation_context;
    }

    if (!cert_validation_config->verifyCertificateHashList().empty()) {
      for (auto hash : cert_validation_config->verifyCertificateHashList()) {
        // Remove colons from the 95 chars long colon-separated "fingerprint"
        // in order to get the hex-encoded string.
        if (hash.size() == 95) {
          hash.erase(std::remove(hash.begin(), hash.end(), ':'), hash.end());
        }
        const auto& decoded = Hex::decode(hash);
        if (decoded.size() != SHA256_DIGEST_LENGTH) {
          throw EnvoyException(absl::StrCat("Invalid hex-encoded SHA-256 ", hash));
        }
        verify_certificate_hash_list_.push_back(decoded);
      }
      verify_mode = verify_mode_validation_context;
    }

    if (!cert_validation_config->verifyCertificateSpkiList().empty()) {
      for (const auto& hash : cert_validation_config->verifyCertificateSpkiList()) {
        const auto decoded = Base64::decode(hash);
        if (decoded.size() != SHA256_DIGEST_LENGTH) {
          throw EnvoyException(absl::StrCat("Invalid base64-encoded SHA-256 ", hash));
        }
        verify_certificate_spki_list_.emplace_back(decoded.begin(), decoded.end());
      }
      verify_mode = verify_mode_validation_context;
    }
  }

  return verify_mode;
}