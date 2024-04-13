bool DefaultCertValidator::matchSubjectAltName(
    X509* cert, const std::vector<SanMatcherPtr>& subject_alt_name_matchers) {
  bssl::UniquePtr<GENERAL_NAMES> san_names(
      static_cast<GENERAL_NAMES*>(X509_get_ext_d2i(cert, NID_subject_alt_name, nullptr, nullptr)));
  if (san_names == nullptr) {
    return false;
  }
  for (const auto& config_san_matcher : subject_alt_name_matchers) {
    for (const GENERAL_NAME* general_name : san_names.get()) {
      if (config_san_matcher->match(general_name)) {
        return true;
      }
    }
  }
  return false;
}