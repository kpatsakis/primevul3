TEST_P(SslSocketTest, CipherSuites) {
  envoy::config::listener::v3::Listener listener;
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;

  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();
  server_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"));
  server_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"));
  updateFilterChain(tls_context, *filter_chain);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;
  envoy::extensions::transport_sockets::tls::v3::TlsParameters* client_params =
      client.mutable_common_tls_context()->mutable_tls_params();

  // Connection using defaults (client & server) succeeds.
  TestUtilOptionsV2 test_options(listener, client, true, GetParam());
  testUtilV2(test_options);

  // Connection using defaults (client & server) succeeds, even with client renegotiation.
  client.set_allow_renegotiation(true);
  testUtilV2(test_options);
  client.set_allow_renegotiation(false);

  // Client connects with one of the supported cipher suites, connection succeeds.
  std::string common_cipher_suite = "ECDHE-RSA-CHACHA20-POLY1305";
  client_params->add_cipher_suites(common_cipher_suite);
  envoy::extensions::transport_sockets::tls::v3::TlsParameters* server_params =
      tls_context.mutable_common_tls_context()->mutable_tls_params();
  server_params->add_cipher_suites(common_cipher_suite);
  server_params->add_cipher_suites("ECDHE-RSA-AES128-GCM-SHA256");
  updateFilterChain(tls_context, *filter_chain);
  TestUtilOptionsV2 cipher_test_options(listener, client, true, GetParam());
  cipher_test_options.setExpectedCiphersuite(common_cipher_suite);
  std::string stats = "ssl.ciphers." + common_cipher_suite;
  cipher_test_options.setExpectedServerStats(stats).setExpectedClientStats(stats);
  testUtilV2(cipher_test_options);
  client_params->clear_cipher_suites();
  server_params->clear_cipher_suites();

  // Client connects with unsupported cipher suite, connection fails.
  client_params->add_cipher_suites("ECDHE-RSA-AES128-GCM-SHA256");
  server_params->add_cipher_suites("ECDHE-RSA-CHACHA20-POLY1305");
  updateFilterChain(tls_context, *filter_chain);
  TestUtilOptionsV2 error_test_options(listener, client, false, GetParam());
  error_test_options.setExpectedServerStats("ssl.connection_error");
  testUtilV2(error_test_options);
  client_params->clear_cipher_suites();
  server_params->clear_cipher_suites();

  // Client connects to a server offering only deprecated cipher suites, connection fails.
  server_params->add_cipher_suites("ECDHE-RSA-AES128-SHA");
  updateFilterChain(tls_context, *filter_chain);
  error_test_options.setExpectedServerStats("ssl.connection_error");
  testUtilV2(error_test_options);
  server_params->clear_cipher_suites();
  updateFilterChain(tls_context, *filter_chain);
  // Verify that ECDHE-RSA-CHACHA20-POLY1305 is not offered by default in FIPS builds.
  client_params->add_cipher_suites(common_cipher_suite);
#ifdef BORINGSSL_FIPS
  testUtilV2(error_test_options);
#else
  testUtilV2(cipher_test_options);
#endif
  client_params->clear_cipher_suites();
}