TEST_P(SslSocketTest, FailedClientCertificateSpkiVerificationNoClientCertificate) {
  envoy::config::listener::v3::Listener listener;
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();
  server_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"));
  server_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"));
  envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext*
      server_validation_ctx =
          tls_context.mutable_common_tls_context()->mutable_validation_context();
  server_validation_ctx->mutable_trusted_ca()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"));
  server_validation_ctx->add_verify_certificate_spki(TEST_SAN_DNS_CERT_SPKI);
  server_validation_ctx->add_verify_certificate_spki(TEST_SAN_URI_CERT_SPKI);
  updateFilterChain(tls_context, *filter_chain);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;
  TestUtilOptionsV2 test_options(listener, client, false, GetParam());
  testUtilV2(test_options.setExpectedServerStats("ssl.fail_verify_no_cert")
                 .setExpectedTransportFailureReasonContains("SSLV3_ALERT_HANDSHAKE_FAILURE"));

  // Fails even with client renegotiation.
  client.set_allow_renegotiation(true);
  testUtilV2(test_options);
}