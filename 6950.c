TEST_P(SslSocketTest, TicketSessionResumptionDifferentVerifyCertHash) {
  const std::string server_ctx_yaml1 = absl::StrCat(R"EOF(
  session_ticket_keys:
    keys:
      filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ticket_key_a"
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
      verify_certificate_hash:
        - ")EOF",
                                                    TEST_SAN_URI_CERT_256_HASH, "\"");

  const std::string server_ctx_yaml2 = absl::StrCat(R"EOF(
  session_ticket_keys:
    keys:
      filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ticket_key_a"
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
      verify_certificate_hash:
        - "0000000000000000000000000000000000000000000000000000000000000000"
        - ")EOF",
                                                    TEST_SAN_URI_CERT_256_HASH, "\"");

  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_key.pem"
)EOF";

  testTicketSessionResumption(server_ctx_yaml1, {}, server_ctx_yaml1, {}, client_ctx_yaml, true,
                              GetParam());
  testTicketSessionResumption(server_ctx_yaml1, {}, server_ctx_yaml2, {}, client_ctx_yaml, false,
                              GetParam());
}