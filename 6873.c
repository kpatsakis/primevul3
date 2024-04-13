void DefaultCertValidator::addClientValidationContext(SSL_CTX* ctx, bool require_client_cert) {
  if (config_ == nullptr || config_->caCert().empty()) {
    return;
  }

  bssl::UniquePtr<BIO> bio(
      BIO_new_mem_buf(const_cast<char*>(config_->caCert().data()), config_->caCert().size()));
  RELEASE_ASSERT(bio != nullptr, "");
  // Based on BoringSSL's SSL_add_file_cert_subjects_to_stack().
  bssl::UniquePtr<STACK_OF(X509_NAME)> list(sk_X509_NAME_new(
      [](const X509_NAME** a, const X509_NAME** b) -> int { return X509_NAME_cmp(*a, *b); }));
  RELEASE_ASSERT(list != nullptr, "");
  for (;;) {
    bssl::UniquePtr<X509> cert(PEM_read_bio_X509(bio.get(), nullptr, nullptr, nullptr));
    if (cert == nullptr) {
      break;
    }
    X509_NAME* name = X509_get_subject_name(cert.get());
    if (name == nullptr) {
      throw EnvoyException(absl::StrCat("Failed to load trusted client CA certificates from ",
                                        config_->caCertPath()));
    }
    // Check for duplicates.
    if (sk_X509_NAME_find(list.get(), nullptr, name)) {
      continue;
    }
    bssl::UniquePtr<X509_NAME> name_dup(X509_NAME_dup(name));
    if (name_dup == nullptr || !sk_X509_NAME_push(list.get(), name_dup.release())) {
      throw EnvoyException(absl::StrCat("Failed to load trusted client CA certificates from ",
                                        config_->caCertPath()));
    }
  }

  // Check for EOF.
  const uint32_t err = ERR_peek_last_error();
  if (ERR_GET_LIB(err) == ERR_LIB_PEM && ERR_GET_REASON(err) == PEM_R_NO_START_LINE) {
    ERR_clear_error();
  } else {
    throw EnvoyException(
        absl::StrCat("Failed to load trusted client CA certificates from ", config_->caCertPath()));
  }
  SSL_CTX_set_client_CA_list(ctx, list.release());

  if (require_client_cert) {
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, nullptr);
  }
}