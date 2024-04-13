TEST_P(SslSocketTest, StatelessSessionResumptionEnabledByDefault) {
  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
)EOF";

  const std::string client_ctx_yaml = R"EOF(
    common_tls_context:
  )EOF";

  testSupportForStatelessSessionResumption(server_ctx_yaml, client_ctx_yaml, true, GetParam());
}