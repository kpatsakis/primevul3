TEST_P(SslSocketTest, RsaPrivateKeyProviderAsyncSignSuccess) {
  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key_provider:
        provider_name: test
        typed_config:
          "@type": type.googleapis.com/google.protobuf.Struct
          value:
            private_key_file: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
            expected_operation: sign
            sync_mode: false
            mode: rsa
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
      crl:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.crl"
)EOF";
  const std::string successful_client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_params:
      cipher_suites:
      - ECDHE-RSA-AES128-GCM-SHA256
)EOF";

  TestUtilOptions successful_test_options(successful_client_ctx_yaml, server_ctx_yaml, true,
                                          GetParam());
  testUtil(successful_test_options.setPrivateKeyMethodExpected(true));
}