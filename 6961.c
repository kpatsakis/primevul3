TEST_P(SslSocketTest, TestOcspStapleOmittedOnSkipStaplingAndResponseExpired) {
  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
    - certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/ocsp/test_data/good_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/ocsp/test_data/good_key.pem"
      ocsp_staple:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/ocsp/test_data/unknown_ocsp_resp.der"
  ocsp_staple_policy: lenient_stapling
  )EOF";

  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_params:
      cipher_suites:
      - TLS_RSA_WITH_AES_128_GCM_SHA256
)EOF";
  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(test_options.setExpectedServerStats("ssl.ocsp_staple_omitted").enableOcspStapling());
}