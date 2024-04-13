TEST_P(SslSocketTest, SignatureAlgorithms) {
  envoy::config::listener::v3::Listener listener;
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext*
      server_validation_ctx =
          tls_context.mutable_common_tls_context()->mutable_validation_context();

  server_validation_ctx->mutable_trusted_ca()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"));
  // Server ECDSA certificate.
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();
  server_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir "
      "}}/test/extensions/transport_sockets/tls/test_data/selfsigned_ecdsa_p256_cert.pem"));
  server_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir "
      "}}/test/extensions/transport_sockets/tls/test_data/selfsigned_ecdsa_p256_key.pem"));
  updateFilterChain(tls_context, *filter_chain);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;
  // Client RSA certificate.
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* client_cert =
      client.mutable_common_tls_context()->add_tls_certificates();
  client_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_cert.pem"));
  client_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_key.pem"));

  // Connection using defaults (client & server) succeeds.
  TestUtilOptionsV2 algorithm_test_options(listener, client, true, GetParam());
  algorithm_test_options.setExpectedClientCertUri("spiffe://lyft.com/test-team")
      .setExpectedServerStats("ssl.sigalgs.rsa_pss_rsae_sha256")
      .setExpectedClientStats("ssl.sigalgs.ecdsa_secp256r1_sha256");
  testUtilV2(algorithm_test_options);

  // Connection using defaults (client & server) succeeds, even with client renegotiation.
  client.set_allow_renegotiation(true);
  testUtilV2(algorithm_test_options);
  client.set_allow_renegotiation(false);
}