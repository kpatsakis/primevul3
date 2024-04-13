TEST_P(SslSocketTest, ClientSessionResumptionDisabledTls12) {
  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_params:
      tls_minimum_protocol_version: TLSv1_0
      tls_maximum_protocol_version: TLSv1_2
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
)EOF";

  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
  max_session_keys: 0
)EOF";

  testClientSessionResumption(server_ctx_yaml, client_ctx_yaml, false, GetParam());
}