TEST_P(SslSocketTest, GetPeerCertChain) {
  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/no_san_chain.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/no_san_key.pem"
)EOF";

  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
  require_client_certificate: true
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  std::string expected_peer_cert_chain =
      TestEnvironment::readFileToStringForTest(TestEnvironment::substitute(
          "{{ test_rundir "
          "}}/test/extensions/transport_sockets/tls/test_data/no_san_chain.pem"));
  testUtil(test_options.setExpectedSerialNumber(TEST_NO_SAN_CERT_SERIAL)
               .setExpectedPeerCertChain(expected_peer_cert_chain));
}