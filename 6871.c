TEST_P(SslSocketTest, RsaPrivateKeyProviderAsyncSignCompleteFailure) {
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
            async_method_error: true
            mode: rsa
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
      crl:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.crl"
)EOF";
  const std::string failing_client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_params:
      cipher_suites:
      - ECDHE-RSA-AES128-GCM-SHA256
)EOF";

  TestUtilOptions failing_test_options(failing_client_ctx_yaml, server_ctx_yaml, false, GetParam());
  testUtil(failing_test_options.setPrivateKeyMethodExpected(true)
               .setExpectedServerCloseEvent(Network::ConnectionEvent::LocalClose)
               .setExpectedServerStats("ssl.connection_error"));
}