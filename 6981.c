TEST_P(SslSocketTest, TicketSessionResumptionDifferentServerNames) {
  const std::string server_ctx_yaml1 = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"
  session_ticket_keys:
    keys:
      filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ticket_key_a"
)EOF";

  const std::string server_ctx_yaml2 = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"
  session_ticket_keys:
    keys:
      filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ticket_key_a"
)EOF";

  std::vector<std::string> server_names1 = {"server1.example.com"};

  const std::string client_ctx_yaml = R"EOF(
    common_tls_context:
  )EOF";

  testTicketSessionResumption(server_ctx_yaml1, server_names1, server_ctx_yaml2, {},
                              client_ctx_yaml, false, GetParam());
}