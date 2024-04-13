TEST_P(SslSocketTest, RsaPrivateKeyProviderMultiCertFail) {
  const std::string client_ctx_yaml = absl::StrCat(R"EOF(
    common_tls_context:
      tls_params:
        tls_minimum_protocol_version: TLSv1_2
        tls_maximum_protocol_version: TLSv1_2
        cipher_suites:
        - ECDHE-ECDSA-AES128-GCM-SHA256
        - ECDHE-RSA-AES128-GCM-SHA256
      validation_context:
        verify_certificate_hash: )EOF",
                                                   TEST_SELFSIGNED_ECDSA_P256_CERT_256_HASH);

  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
    - certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/selfsigned_cert.pem"
      private_key_provider:
        provider_name: test
        typed_config:
          "@type": type.googleapis.com/google.protobuf.Struct
          value:
            private_key_file: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
            expected_operation: sign
            sync_mode: false
            mode: rsa
    - certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/selfsigned_ecdsa_p256_cert.pem"
      private_key_provider:
        provider_name: test
        typed_config:
          "@type": type.googleapis.com/google.protobuf.Struct
          value:
            private_key_file: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/selfsigned_ecdsa_p256_key.pem"
            expected_operation: sign
            sync_mode: false
            mode: rsa
)EOF";

  TestUtilOptions failing_test_options(client_ctx_yaml, server_ctx_yaml, false, GetParam());
  EXPECT_THROW_WITH_MESSAGE(testUtil(failing_test_options.setPrivateKeyMethodExpected(true)),
                            EnvoyException, "Private key is not RSA.")
}