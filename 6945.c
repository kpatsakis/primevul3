TEST_P(SslSocketTest, ClientCertificateHashVerificationNoCA) {
  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_key.pem"
)EOF";

  const std::string server_ctx_yaml = absl::StrCat(R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
    validation_context:
      verify_certificate_hash: ")EOF",
                                                   TEST_SAN_URI_CERT_256_HASH, "\"");

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(test_options.setExpectedClientCertUri("spiffe://lyft.com/test-team")
               .setExpectedSerialNumber(TEST_SAN_URI_CERT_SERIAL));
}