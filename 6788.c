void DefaultCertValidator::updateDigestForSessionId(bssl::ScopedEVP_MD_CTX& md,
                                                    uint8_t hash_buffer[EVP_MAX_MD_SIZE],
                                                    unsigned hash_length) {
  int rc;

  // Hash all the settings that affect whether the server will allow/accept
  // the client connection. This ensures that the client is always validated against
  // the correct settings, even if session resumption across different listeners
  // is enabled.
  if (ca_cert_ != nullptr) {
    rc = X509_digest(ca_cert_.get(), EVP_sha256(), hash_buffer, &hash_length);
    RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));
    RELEASE_ASSERT(hash_length == SHA256_DIGEST_LENGTH,
                   fmt::format("invalid SHA256 hash length {}", hash_length));

    rc = EVP_DigestUpdate(md.get(), hash_buffer, hash_length);
    RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));
  }

  for (const auto& hash : verify_certificate_hash_list_) {
    rc = EVP_DigestUpdate(md.get(), hash.data(),
                          hash.size() *
                              sizeof(std::remove_reference<decltype(hash)>::type::value_type));
    RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));
  }

  for (const auto& hash : verify_certificate_spki_list_) {
    rc = EVP_DigestUpdate(md.get(), hash.data(),
                          hash.size() *
                              sizeof(std::remove_reference<decltype(hash)>::type::value_type));
    RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));
  }

  rc = EVP_DigestUpdate(md.get(), &verify_trusted_ca_, sizeof(verify_trusted_ca_));
  RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));

  if (config_ != nullptr) {
    for (const auto& matcher : config_->subjectAltNameMatchers()) {
      size_t hash = MessageUtil::hash(matcher);
      rc = EVP_DigestUpdate(md.get(), &hash, sizeof(hash));
      RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));
    }

    const std::string& crl = config_->certificateRevocationList();
    if (!crl.empty()) {
      rc = EVP_DigestUpdate(md.get(), crl.data(), crl.length());
      RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));
    }

    bool allow_expired = config_->allowExpiredCertificate();
    rc = EVP_DigestUpdate(md.get(), &allow_expired, sizeof(allow_expired));
    RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));

    auto trust_chain_verification = config_->trustChainVerification();
    rc = EVP_DigestUpdate(md.get(), &trust_chain_verification, sizeof(trust_chain_verification));
    RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));

    auto only_leaf_crl = config_->onlyVerifyLeafCertificateCrl();
    rc = EVP_DigestUpdate(md.get(), &only_leaf_crl, sizeof(only_leaf_crl));
    RELEASE_ASSERT(rc == 1, Utility::getLastCryptoError().value_or(""));
  }
}