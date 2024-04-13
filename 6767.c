TEST_P(SslSocketTest, TicketSessionResumptionCustomTimeout) {
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
  session_ticket_keys:
    keys:
      filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ticket_key_a"
  session_timeout: 2307s
)EOF";

  const std::string client_ctx_yaml = R"EOF(
    common_tls_context:
  )EOF";

  testTicketSessionResumption(server_ctx_yaml, {}, server_ctx_yaml, {}, client_ctx_yaml, true,
                              GetParam(), 2307);
}