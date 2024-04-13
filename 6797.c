TEST_P(SslSocketTest, ALPN) {
  envoy::config::listener::v3::Listener listener;
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();
  server_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"));
  server_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"));
  envoy::extensions::transport_sockets::tls::v3::CommonTlsContext* server_ctx =
      tls_context.mutable_common_tls_context();
  updateFilterChain(tls_context, *filter_chain);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;
  envoy::extensions::transport_sockets::tls::v3::CommonTlsContext* client_ctx =
      client.mutable_common_tls_context();

  // Connection using defaults (client & server) succeeds, no ALPN is negotiated.
  TestUtilOptionsV2 test_options(listener, client, true, GetParam());
  testUtilV2(test_options);

  // Connection using defaults (client & server) succeeds, no ALPN is negotiated,
  // even with client renegotiation.
  client.set_allow_renegotiation(true);
  testUtilV2(test_options);
  client.set_allow_renegotiation(false);

  // Client connects without ALPN to a server with "test" ALPN, no ALPN is negotiated.
  server_ctx->add_alpn_protocols("test");
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(test_options);
  server_ctx->clear_alpn_protocols();

  // Client connects with "test" ALPN to a server without ALPN, no ALPN is negotiated.
  client_ctx->add_alpn_protocols("test");
  testUtilV2(test_options);

  client_ctx->clear_alpn_protocols();

  // Client connects with "test" ALPN to a server with "test" ALPN, "test" ALPN is negotiated.
  client_ctx->add_alpn_protocols("test");
  server_ctx->add_alpn_protocols("test");
  updateFilterChain(tls_context, *filter_chain);
  test_options.setExpectedALPNProtocol("test");
  testUtilV2(test_options);
  test_options.setExpectedALPNProtocol("");
  client_ctx->clear_alpn_protocols();
  server_ctx->clear_alpn_protocols();

  // Client connects with "test" ALPN to a server with "test" ALPN, "test" ALPN is negotiated,
  // even with client renegotiation.
  client.set_allow_renegotiation(true);
  client_ctx->add_alpn_protocols("test");
  server_ctx->add_alpn_protocols("test");
  updateFilterChain(tls_context, *filter_chain);
  test_options.setExpectedALPNProtocol("test");
  testUtilV2(test_options);
  test_options.setExpectedALPNProtocol("");
  client.set_allow_renegotiation(false);
  client_ctx->clear_alpn_protocols();
  server_ctx->clear_alpn_protocols();

  // Client connects with "test" ALPN to a server with "test2" ALPN, no ALPN is negotiated.
  client_ctx->add_alpn_protocols("test");
  server_ctx->add_alpn_protocols("test2");
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(test_options);
  client_ctx->clear_alpn_protocols();
  server_ctx->clear_alpn_protocols();

  // Client attempts to configure ALPN that is too large.
  client_ctx->add_alpn_protocols(std::string(100000, 'a'));
  EXPECT_THROW_WITH_MESSAGE(testUtilV2(test_options), EnvoyException,
                            "Invalid ALPN protocol string");
}