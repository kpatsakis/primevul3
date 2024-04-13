TEST_P(SslSocketTest, GetCertDigestServerCertWithoutCommonName) {
  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/no_san_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/no_san_key.pem"
)EOF";

  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_only_dns_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_only_dns_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(test_options.setExpectedSha256Digest(TEST_NO_SAN_CERT_256_HASH)
               .setExpectedSha1Digest(TEST_NO_SAN_CERT_1_HASH)
               .setExpectedSerialNumber(TEST_NO_SAN_CERT_SERIAL));
}