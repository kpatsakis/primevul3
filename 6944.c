TEST_P(SslSocketTest, FailedClientCertificateDefaultExpirationVerification) {
  envoy::config::listener::v3::Listener listener;
  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;

  configureServerAndExpiredClientCertificate(listener, client, /*server_config=*/{});

  TestUtilOptionsV2 test_options(listener, client, false, GetParam());
  testUtilV2(test_options.setExpectedClientCertUri("spiffe://lyft.com/test-team")
                 .setExpectedTransportFailureReasonContains("SSLV3_ALERT_CERTIFICATE_EXPIRED"));
}