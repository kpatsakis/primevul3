TEST_P(SslSocketTest, GetCertDigestInvalidFiles) {
  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
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
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(
      test_options.setExpectedSha256Digest("").setExpectedSha1Digest("").setExpectedSerialNumber(
          ""));
}