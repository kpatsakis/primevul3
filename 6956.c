TEST_P(SslSocketTest, Ipv4San) {
  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/config/integration/certs/upstreamcacert.pem"
      match_typed_subject_alt_names:
      - san_type: IP_ADDRESS
        matcher:
          exact: "127.0.0.1"
)EOF";

  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/config/integration/certs/upstreamlocalhostcert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/config/integration/certs/upstreamlocalhostkey.pem"
)EOF";

  TestUtilOptions test_options(client_ctx_yaml, server_ctx_yaml, true, GetParam());
  testUtil(test_options);
}