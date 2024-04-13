TEST_P(SslSocketTest, Pkcs12CertificatesWithPassword) {
  envoy::config::listener::v3::Listener listener;
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();

  server_cert->mutable_pkcs12()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir "
      "}}/test/extensions/transport_sockets/tls/test_data/password_protected_certkey.p12"));
  server_cert->mutable_password()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir "
      "}}/test/extensions/transport_sockets/tls/test_data/password_protected_password.txt"));
  envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext*
      server_validation_ctx =
          tls_context.mutable_common_tls_context()->mutable_validation_context();
  server_validation_ctx->mutable_trusted_ca()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"));
  server_validation_ctx->add_verify_certificate_hash(
      "0000000000000000000000000000000000000000000000000000000000000000");
  server_validation_ctx->add_verify_certificate_hash(TEST_PASSWORD_PROTECTED_CERT_256_HASH);
  updateFilterChain(tls_context, *filter_chain);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* client_cert =
      client.mutable_common_tls_context()->add_tls_certificates();
  client_cert->mutable_pkcs12()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir "
      "}}/test/extensions/transport_sockets/tls/test_data/password_protected_certkey.p12"));
  client_cert->mutable_password()->set_inline_string(
      TestEnvironment::readFileToStringForTest(TestEnvironment::substitute(
          "{{ test_rundir "
          "}}/test/extensions/transport_sockets/tls/test_data/password_protected_password.txt")));

  TestUtilOptionsV2 test_options(listener, client, true, GetParam());
  testUtilV2(test_options.setExpectedClientCertUri("spiffe://lyft.com/test-team")
                 .setExpectedServerCertDigest(TEST_PASSWORD_PROTECTED_CERT_256_HASH));

  // Works even with client renegotiation.
  client.set_allow_renegotiation(true);
  testUtilV2(test_options);
}