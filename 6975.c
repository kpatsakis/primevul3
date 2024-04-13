TEST_P(SslSocketTest, FailedClientAuthSanVerificationNoClientCert) {
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
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
      match_typed_subject_alt_names:
      - san_type: DNS
        matcher:
          exact: "example.com"
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, false, GetParam());
  testUtil(test_options.setExpectedServerStats("ssl.fail_verify_no_cert"));
}