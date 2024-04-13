void configureServerAndExpiredClientCertificate(
    envoy::config::listener::v3::Listener& listener,
    envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext& client,
    const OptionalServerConfig& server_config) {
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();
  server_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"));
  server_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"));

  envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext*
      server_validation_ctx =
          tls_context.mutable_common_tls_context()->mutable_validation_context();
  if (server_config.trusted_ca.has_value()) {
    server_validation_ctx->mutable_trusted_ca()->set_filename(
        TestEnvironment::substitute(server_config.trusted_ca.value()));
  } else {
    server_validation_ctx->mutable_trusted_ca()->set_filename(TestEnvironment::substitute(
        "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"));
  }
  if (server_config.allow_expired_cert.has_value()) {
    server_validation_ctx->set_allow_expired_certificate(server_config.allow_expired_cert.value());
  }
  if (server_config.cert_hash.has_value()) {
    server_validation_ctx->add_verify_certificate_hash(server_config.cert_hash.value());
  }
  updateFilterChain(tls_context, *filter_chain);

  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* client_cert =
      client.mutable_common_tls_context()->add_tls_certificates();
  client_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir "
      "}}/test/extensions/transport_sockets/tls/test_data/expired_san_uri_cert.pem"));
  client_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/expired_san_uri_key.pem"));
}