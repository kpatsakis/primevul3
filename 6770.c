TEST_P(SslSocketTest, GetCertDigestInline) {
  envoy::config::listener::v3::Listener listener;
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();

  // From test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem.
  server_cert->mutable_certificate_chain()->set_inline_bytes(
      TestEnvironment::readFileToStringForTest(TestEnvironment::substitute(
          "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem")));

  // From test/extensions/transport_sockets/tls/test_data/san_dns_key.pem.
  server_cert->mutable_private_key()->set_inline_bytes(
      TestEnvironment::readFileToStringForTest(TestEnvironment::substitute(
          "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem")));

  envoy::extensions::transport_sockets::tls::v3::CertificateValidationContext*
      server_validation_ctx =
          tls_context.mutable_common_tls_context()->mutable_validation_context();
  // From test/extensions/transport_sockets/tls/test_data/ca_certificates.pem.
  server_validation_ctx->mutable_trusted_ca()->set_inline_bytes(
      TestEnvironment::readFileToStringForTest(TestEnvironment::substitute(
          "{{ test_rundir "
          "}}/test/extensions/transport_sockets/tls/test_data/ca_certificates.pem")));

  updateFilterChain(tls_context, *filter_chain);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client_ctx;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* client_cert =
      client_ctx.mutable_common_tls_context()->add_tls_certificates();

  // From test/extensions/transport_sockets/tls/test_data/san_uri_cert.pem.
  client_cert->mutable_certificate_chain()->set_inline_bytes(
      TestEnvironment::readFileToStringForTest(TestEnvironment::substitute(
          "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_cert.pem")));

  // From test/extensions/transport_sockets/tls/test_data/san_uri_key.pem.
  client_cert->mutable_private_key()->set_inline_bytes(
      TestEnvironment::readFileToStringForTest(TestEnvironment::substitute(
          "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_key.pem")));

  TestUtilOptionsV2 test_options(listener, client_ctx, true, GetParam());
  testUtilV2(test_options.setExpectedClientCertUri("spiffe://lyft.com/test-team")
                 .setExpectedServerCertDigest(TEST_SAN_DNS_CERT_256_HASH));
}