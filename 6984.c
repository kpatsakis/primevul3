TEST_P(SslSocketTest, TicketSessionResumptionWithClientCA) {
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
  session_ticket_keys:
    keys:
      filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ticket_key_a"
)EOF";

  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/no_san_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/no_san_key.pem"
)EOF";

  testTicketSessionResumption(server_ctx_yaml, {}, server_ctx_yaml, {}, client_ctx_yaml, true,
                              GetParam());
}