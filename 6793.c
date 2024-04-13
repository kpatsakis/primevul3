TEST_P(SslSocketTest, NoCert) {
  const std::string client_ctx_yaml = R"EOF(
    common_tls_context:
  )EOF";

  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(test_options.setExpectedServerStats("ssl.no_certificate")
               .setExpectNoCert()
               .setExpectNoCertChain());
}