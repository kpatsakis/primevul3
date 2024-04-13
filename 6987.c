TEST_P(SslSocketTest, NoCertUntrustedPermitted) {
  const std::string client_ctx_yaml = R"EOF(
    common_tls_context:
  )EOF";

  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/fake_ca_cert.pem"
      trust_chain_verification: ACCEPT_UNTRUSTED
      verify_certificate_hash: "0000000000000000000000000000000000000000000000000000000000000000"
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(test_options.setExpectedServerStats("ssl.no_certificate")
               .setExpectNoCert()
               .setExpectNoCertChain());
}