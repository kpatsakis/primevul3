size_t DefaultCertValidator::daysUntilFirstCertExpires() const {
  return Utility::getDaysUntilExpiration(ca_cert_.get(), time_source_);
}