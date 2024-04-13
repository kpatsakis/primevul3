TEST_P(SslSocketTest, RevokedIntermediateCertificate) {

  // This should succeed, since the crl chain is complete.
  //
  // Trust chain contains:
  //  - Root authority certificate (i.e., ca_cert.pem)
  //  - Intermediate authority certificate (i.e., intermediate_ca_cert.pem)
  //
  // Certificate revocation list contains:
  //  - Root authority certificate revocation list (i.e., ca_cert.crl)
  //  - Intermediate authority certificate revocation list (i.e., intermediate_ca_cert.crl)
  const std::string complete_server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/intermediate_ca_cert_chain.pem"
      crl:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/intermediate_ca_cert_chain.crl"
)EOF";

  // This should fail, since the crl chain is incomplete.
  //
  // Trust chain contains:
  //  - Root authority certificate (i.e., ca_cert.pem)
  //  - Intermediate authority certificate (i.e., intermediate_ca_cert.pem)
  //
  // Certificate revocation list contains:
  //  - Root authority certificate revocation list (i.e., ca_cert.crl)
  //
  // Certificate revocation list omits:
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
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/intermediate_ca_cert_chain.pem"
      crl:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/intermediate_ca_cert.crl"
)EOF";

  // This should fail, since the certificate has been revoked.
  const std::string revoked_client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns3_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns3_key.pem"
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

  // Ensure that incomplete crl chains fail with revoked certificates.
  TestUtilOptions incomplete_revoked_test_options(revoked_client_ctx_yaml,
                                                  incomplete_server_ctx_yaml, false, GetParam());
  testUtil(incomplete_revoked_test_options.setExpectedServerStats("ssl.fail_verify_error")
               .setExpectedVerifyErrorCode(X509_V_ERR_CERT_REVOKED));

  // Ensure that incomplete crl chains fail with unrevoked certificates.
  TestUtilOptions incomplete_unrevoked_test_options(unrevoked_client_ctx_yaml,
                                                    incomplete_server_ctx_yaml, false, GetParam());
  testUtil(incomplete_unrevoked_test_options.setExpectedServerStats("ssl.fail_verify_error")
               .setExpectedVerifyErrorCode(X509_V_ERR_UNABLE_TO_GET_CRL));

  // Ensure that complete crl chains fail with revoked certificates.
  TestUtilOptions complete_revoked_test_options(revoked_client_ctx_yaml, complete_server_ctx_yaml,
                                                false, GetParam());
  testUtil(complete_revoked_test_options.setExpectedServerStats("ssl.fail_verify_error")
               .setExpectedVerifyErrorCode(X509_V_ERR_CERT_REVOKED));

  // Ensure that complete crl chains succeed with unrevoked certificates.
  TestUtilOptions complete_unrevoked_test_options(unrevoked_client_ctx_yaml,
                                                  complete_server_ctx_yaml, true, GetParam());
  testUtil(complete_unrevoked_test_options.setExpectedSerialNumber(TEST_SAN_DNS4_CERT_SERIAL));
}