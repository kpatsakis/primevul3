TEST_P(SslSocketTest, NoCertUntrustedNotPermitted) {
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
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/fake_ca_cert.pem"
      trust_chain_verification: VERIFY_TRUST_CHAIN
      verify_certificate_hash: "0000000000000000000000000000000000000000000000000000000000000000"
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, false, GetParam());
  testUtil(test_options.setExpectedServerStats("ssl.fail_verify_no_cert"));
}