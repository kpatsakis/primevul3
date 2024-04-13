TEST_P(SslSocketTest, NotRevokedLeafCertificateOnlyLeafCRLValidation) {
  // The test checks that revoked certificate will makes the validation success even if we set
  // only_verify_leaf_cert_crl to true.
  //
  // Trust chain contains:
  //  - Root authority certificate (i.e., ca_cert.pem)
  //  - Intermediate authority certificate (i.e., intermediate_ca_cert.pem)
  //  - Intermediate authority certificate revocation list (i.e., intermediate_ca_cert.crl)
  //
  // Trust chain omits (But this test will succeed):
  //  - Root authority certificate revocation list (i.e., ca_cert.crl)
  const std::string incomplete_server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/intermediate_ca_cert_chain_with_crl.pem"
      only_verify_leaf_cert_crl: true
)EOF";

  // This should succeed, since the certificate has not been revoked.
  const std::string unrevoked_client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns4_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns4_key.pem"
)EOF";

  TestUtilOptions complete_unrevoked_test_options(unrevoked_client_ctx_yaml,
                                                  incomplete_server_ctx_yaml, true, GetParam());
  testUtil(complete_unrevoked_test_options.setExpectedSerialNumber(TEST_SAN_DNS4_CERT_SERIAL));
}