TEST_P(SslSocketTest, GetSubjectsWithBothCerts) {
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
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_uri_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
  require_client_certificate: true
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(test_options.setExpectedSerialNumber(TEST_NO_SAN_CERT_SERIAL)
               .setExpectedPeerIssuer(
                   "CN=Test CA,OU=Lyft Engineering,O=Lyft,L=San Francisco,ST=California,C=US")
               .setExpectedPeerSubject(
                   "CN=Test Server,OU=Lyft Engineering,O=Lyft,L=San Francisco,ST=California,C=US")
               .setExpectedLocalSubject(
                   "CN=Test Server,OU=Lyft Engineering,O=Lyft,L=San Francisco,ST=California,C=US"));
}