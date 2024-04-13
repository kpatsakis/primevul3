int DefaultCertValidator::doVerifyCertChain(
    X509_STORE_CTX* store_ctx, Ssl::SslExtendedSocketInfo* ssl_extended_info, X509& leaf_cert,
    const Network::TransportSocketOptions* transport_socket_options) {
  if (verify_trusted_ca_) {
    int ret = X509_verify_cert(store_ctx);
    if (ssl_extended_info) {
      ssl_extended_info->setCertificateValidationStatus(
          ret == 1 ? Envoy::Ssl::ClientValidationStatus::Validated
                   : Envoy::Ssl::ClientValidationStatus::Failed);
    }

    if (ret <= 0) {
      stats_.fail_verify_error_.inc();
      ENVOY_LOG(debug, "{}", Utility::getX509VerificationErrorInfo(store_ctx));
      return allow_untrusted_certificate_ ? 1 : ret;
    }
  }

  Envoy::Ssl::ClientValidationStatus validated =
      verifyCertificate(&leaf_cert,
                        transport_socket_options != nullptr
                            ? transport_socket_options->verifySubjectAltNameListOverride()
                            : std::vector<std::string>{},
                        subject_alt_name_matchers_);

  if (ssl_extended_info) {
    if (ssl_extended_info->certificateValidationStatus() ==
        Envoy::Ssl::ClientValidationStatus::NotValidated) {
      ssl_extended_info->setCertificateValidationStatus(validated);
    } else if (validated != Envoy::Ssl::ClientValidationStatus::NotValidated) {
      ssl_extended_info->setCertificateValidationStatus(validated);
    }
  }

  // If `trusted_ca` exists, it is already verified in the code above. Thus, we just need to make
  // sure the verification for other validation context configurations doesn't fail (i.e. either
  // `NotValidated` or `Validated`). If `trusted_ca` doesn't exist, we will need to make sure
  // other configurations are verified and the verification succeed.
  int validation_status = verify_trusted_ca_
                              ? validated != Envoy::Ssl::ClientValidationStatus::Failed
                              : validated == Envoy::Ssl::ClientValidationStatus::Validated;

  return allow_untrusted_certificate_ ? 1 : validation_status;
}