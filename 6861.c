TEST_P(SslSocketTest, X509ExtensionsCertificateSerialNumber) {
  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/extensions_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/extensions_key.pem"
)EOF";

  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/extensions_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/extensions_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
  require_client_certificate: true
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(test_options.setExpectedSerialNumber(TEST_EXTENSIONS_CERT_SERIAL));
}