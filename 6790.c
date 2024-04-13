TEST_P(SslSocketTest, TestConnectionFailsWhenCertIsMustStapleAndResponseExpired) {
  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
    - certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/ocsp/test_data/revoked_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/ocsp/test_data/revoked_key.pem"
      ocsp_staple:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/ocsp/test_data/revoked_ocsp_resp.der"
  ocsp_staple_policy: lenient_stapling
  )EOF";

  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_params:
      cipher_suites:
      - TLS_RSA_WITH_AES_128_GCM_SHA256
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, false, GetParam());
  testUtil(test_options.setExpectedServerStats("ssl.ocsp_staple_failed").enableOcspStapling());
}