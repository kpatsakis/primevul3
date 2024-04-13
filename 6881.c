Envoy::Ssl::CertificateDetailsPtr DefaultCertValidator::getCaCertInformation() const {
  if (ca_cert_ == nullptr) {
    return nullptr;
  }
  return Utility::certificateDetails(ca_cert_.get(), getCaFileName(), time_source_);
}