DefaultCertValidator::DefaultCertValidator(
    const Envoy::Ssl::CertificateValidationContextConfig* config, SslStats& stats,
    TimeSource& time_source)
    : config_(config), stats_(stats), time_source_(time_source) {
  if (config_ != nullptr) {
    allow_untrusted_certificate_ = config_->trustChainVerification() ==
                                   envoy::extensions::transport_sockets::tls::v3::
                                       CertificateValidationContext::ACCEPT_UNTRUSTED;
  }
};