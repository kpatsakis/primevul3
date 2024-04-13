TEST_P(SslSocketTest, RevokedCertificate) {

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
      crl:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.crl"
)EOF";

  // This should fail, since the certificate has been revoked.
  const std::string revoked_client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"
)EOF";

  TestUtilOptions revoked_test_options(revoked_client_ctx_yaml, server_ctx_yaml, false, GetParam());
  testUtil(revoked_test_options.setExpectedServerStats("ssl.fail_verify_error")
               .setExpectedVerifyErrorCode(X509_V_ERR_CERT_REVOKED));

  // This should succeed, since the cert isn't revoked.
  const std::string successful_client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns2_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns2_key.pem"
)EOF";

  TestUtilOptions successful_test_options(successful_client_ctx_yaml, server_ctx_yaml, true,
                                          GetParam());
  testUtil(successful_test_options.setExpectedSerialNumber(TEST_SAN_DNS2_CERT_SERIAL));
}