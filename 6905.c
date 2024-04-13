TEST_P(SslSocketTest, FailedClientCertAllowServerExpiredWrongCAVerification) {
  envoy::config::listener::v3::Listener listener;
  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;

  OptionalServerConfig server_config;
  server_config.allow_expired_cert = true;
  // Fake CA is not used to sign the client's certificate.
  server_config.trusted_ca = "{{ test_rundir "
                             "}}/test/extensions/transport_sockets/tls/test_data/fake_ca_cert.pem";
  configureServerAndExpiredClientCertificate(listener, client, server_config);

  TestUtilOptionsV2 test_options(listener, client, false, GetParam());
  testUtilV2(test_options.setExpectedClientCertUri("spiffe://lyft.com/test-team")
                 .setExpectedTransportFailureReasonContains("TLSV1_ALERT_UNKNOWN_CA"));
}