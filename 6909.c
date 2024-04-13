bool DefaultCertValidator::verifySubjectAltName(X509* cert,
                                                const std::vector<std::string>& subject_alt_names) {
  bssl::UniquePtr<GENERAL_NAMES> san_names(
      static_cast<GENERAL_NAMES*>(X509_get_ext_d2i(cert, NID_subject_alt_name, nullptr, nullptr)));
  if (san_names == nullptr) {
    return false;
  }
  for (const GENERAL_NAME* general_name : san_names.get()) {
    const std::string san = Utility::generalNameAsString(general_name);
    for (auto& config_san : subject_alt_names) {
      if (general_name->type == GEN_DNS ? Utility::dnsNameMatch(config_san, san.c_str())
                                        : config_san == san) {
        return true;
      }
    }
  }
  return false;
}