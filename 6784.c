TEST_P(SslSocketTest, ProtocolVersions) {
  envoy::config::listener::v3::Listener listener;
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();
  server_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"));
  server_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"));
  envoy::extensions::transport_sockets::tls::v3::TlsParameters* server_params =
      tls_context.mutable_common_tls_context()->mutable_tls_params();

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;
  envoy::extensions::transport_sockets::tls::v3::TlsParameters* client_params =
      client.mutable_common_tls_context()->mutable_tls_params();

  // Note: There aren't any valid TLSv1.0 or TLSv1.1 cipher suites enabled by default,
  // so enable them to avoid false positives.
  client_params->add_cipher_suites("ECDHE-RSA-AES128-SHA");
  server_params->add_cipher_suites("ECDHE-RSA-AES128-SHA");
  updateFilterChain(tls_context, *filter_chain);

  // Connection using defaults (client & server) succeeds, negotiating TLSv1.2.
  TestUtilOptionsV2 tls_v1_2_test_options =
      createProtocolTestOptions(listener, client, GetParam(), "TLSv1.2");
  testUtilV2(tls_v1_2_test_options);

  // Connection using defaults (client & server) succeeds, negotiating TLSv1.2,
  // even with client renegotiation.
  client.set_allow_renegotiation(true);
  testUtilV2(tls_v1_2_test_options);
  client.set_allow_renegotiation(false);

  TestUtilOptionsV2 error_test_options(listener, client, false, GetParam());
  error_test_options.setExpectedServerStats("ssl.connection_error")
      .setExpectedTransportFailureReasonContains("TLSV1_ALERT_PROTOCOL_VERSION");

  // Connection using TLSv1.0 (client) and defaults (server) fails.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  testUtilV2(error_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();

  // Connection using TLSv1.1 (client) and defaults (server) fails.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_1);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_1);
  testUtilV2(error_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();

  // Connection using TLSv1.2 (client) and defaults (server) succeeds.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_2);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_2);
  testUtilV2(tls_v1_2_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();

  // Connection using TLSv1.3 (client) and defaults (server) succeeds.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  TestUtilOptionsV2 tls_v1_3_test_options =
      createProtocolTestOptions(listener, client, GetParam(), "TLSv1.3");
  testUtilV2(tls_v1_3_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();

  // Connection using TLSv1.0-1.3 (client) and defaults (server) succeeds.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  testUtilV2(tls_v1_3_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();

  // Connection using TLSv1.0 (client) and TLSv1.0-1.3 (server) succeeds.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  updateFilterChain(tls_context, *filter_chain);
  TestUtilOptionsV2 tls_v1_test_options =
      createProtocolTestOptions(listener, client, GetParam(), "TLSv1");
  testUtilV2(tls_v1_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();

  // Connection using TLSv1.3 (client) and TLSv1.0-1.3 (server) succeeds.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(tls_v1_3_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();

  TestUtilOptionsV2 unsupported_protocol_test_options(listener, client, false, GetParam());
  unsupported_protocol_test_options.setExpectedServerStats("ssl.connection_error")
      .setExpectedTransportFailureReasonContains("UNSUPPORTED_PROTOCOL");

  // Connection using defaults (client) and TLSv1.0 (server) fails.
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(unsupported_protocol_test_options);
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();

  // Connection using defaults (client) and TLSv1.1 (server) fails.
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_1);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_1);
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(unsupported_protocol_test_options);
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();

  // Connection using defaults (client) and TLSv1.2 (server) succeeds.
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_2);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_2);
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(tls_v1_2_test_options);
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();

  // Connection using defaults (client) and TLSv1.3 (server) fails.
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(error_test_options);
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();

  // Connection using defaults (client) and TLSv1.0-1.3 (server) succeeds.
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(tls_v1_2_test_options);
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();

  // Connection using TLSv1.0-TLSv1.3 (client) and TLSv1.0 (server) succeeds.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(tls_v1_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();

  // Connection using TLSv1.0-TLSv1.3 (client) and TLSv1.3 (server) succeeds.
  client_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_0);
  client_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  server_params->set_tls_minimum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  server_params->set_tls_maximum_protocol_version(
      envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3);
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(tls_v1_3_test_options);
  client_params->clear_tls_minimum_protocol_version();
  client_params->clear_tls_maximum_protocol_version();
  server_params->clear_tls_minimum_protocol_version();
  server_params->clear_tls_maximum_protocol_version();
}