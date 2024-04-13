TEST_P(SslSocketTest, TicketSessionResumptionDifferentVerifyCertSpki) {
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
      verify_certificate_spki:
        - ")EOF",
                                                    TEST_SAN_URI_CERT_SPKI, "\"");

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
      verify_certificate_spki:
        - "NvqYIYSbgK2vCJpQhObf77vv+bQWtc5ek5RIOwPiC9A="
        - ")EOF",
                                                    TEST_SAN_URI_CERT_SPKI, "\"");

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