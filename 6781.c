Envoy::Ssl::ClientValidationStatus DefaultCertValidator::verifyCertificate(
    X509* cert, const std::vector<std::string>& verify_san_list,
    const std::vector<SanMatcherPtr>& subject_alt_name_matchers) {
  Envoy::Ssl::ClientValidationStatus validated = Envoy::Ssl::ClientValidationStatus::NotValidated;

  if (!verify_san_list.empty()) {
    if (!verifySubjectAltName(cert, verify_san_list)) {
      stats_.fail_verify_san_.inc();
      return Envoy::Ssl::ClientValidationStatus::Failed;
    }
    validated = Envoy::Ssl::ClientValidationStatus::Validated;
  }

  if (!subject_alt_name_matchers.empty()) {
    if (!matchSubjectAltName(cert, subject_alt_name_matchers)) {
      stats_.fail_verify_san_.inc();
      return Envoy::Ssl::ClientValidationStatus::Failed;
    }
    validated = Envoy::Ssl::ClientValidationStatus::Validated;
  }

  if (!verify_certificate_hash_list_.empty() || !verify_certificate_spki_list_.empty()) {
    const bool valid_certificate_hash =
        !verify_certificate_hash_list_.empty() &&
        verifyCertificateHashList(cert, verify_certificate_hash_list_);
    const bool valid_certificate_spki =
        !verify_certificate_spki_list_.empty() &&
        verifyCertificateSpkiList(cert, verify_certificate_spki_list_);

    if (!valid_certificate_hash && !valid_certificate_spki) {
      stats_.fail_verify_cert_hash_.inc();
      return Envoy::Ssl::ClientValidationStatus::Failed;
    }

    validated = Envoy::Ssl::ClientValidationStatus::Validated;
  }

  return validated;
}