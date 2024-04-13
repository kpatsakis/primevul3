TEST_P(SslSocketTest, OverrideApplicationProtocols) {
  envoy::config::listener::v3::Listener listener;
  envoy::config::listener::v3::FilterChain* filter_chain = listener.add_filter_chains();
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  envoy::extensions::transport_sockets::tls::v3::TlsCertificate* server_cert =
      tls_context.mutable_common_tls_context()->add_tls_certificates();

  server_cert->mutable_certificate_chain()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_cert.pem"));
  server_cert->mutable_private_key()->set_filename(TestEnvironment::substitute(
      "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/san_dns_key.pem"));

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client;
  TestUtilOptionsV2 test_options(listener, client, true, GetParam());

  // Client connects without ALPN to a server with "test" ALPN, no ALPN is negotiated.
  envoy::extensions::transport_sockets::tls::v3::CommonTlsContext* server_ctx =
      tls_context.mutable_common_tls_context();
  server_ctx->add_alpn_protocols("test");
  updateFilterChain(tls_context, *filter_chain);
  testUtilV2(test_options);
  server_ctx->clear_alpn_protocols();
  // Override client side ALPN, "test" ALPN is used.
  server_ctx->add_alpn_protocols("test");
  updateFilterChain(tls_context, *filter_chain);
  auto transport_socket_options = std::make_shared<Network::TransportSocketOptionsImpl>(
      "", std::vector<std::string>{}, std::vector<std::string>{"foo", "test", "bar"});

  testUtilV2(test_options.setExpectedALPNProtocol("test").setTransportSocketOptions(
      transport_socket_options));

  // Set fallback ALPN on the client side ALPN, "test" ALPN is used since no ALPN is specified
  // in the config.
  server_ctx->add_alpn_protocols("test");
  transport_socket_options = std::make_shared<Network::TransportSocketOptionsImpl>(
      "", std::vector<std::string>{}, std::vector<std::string>{}, std::vector<std::string>{"test"});
  testUtilV2(test_options.setExpectedALPNProtocol("test").setTransportSocketOptions(
      transport_socket_options));

  // With multiple fallbacks specified, a single match will match.
  transport_socket_options = std::make_shared<Network::TransportSocketOptionsImpl>(
      "", std::vector<std::string>{}, std::vector<std::string>{},
      std::vector<std::string>{"foo", "test"});
  testUtilV2(test_options.setExpectedALPNProtocol("test").setTransportSocketOptions(
      transport_socket_options));

  // With multiple matching fallbacks specified, a single match will match.
  server_ctx->add_alpn_protocols("foo");
  transport_socket_options = std::make_shared<Network::TransportSocketOptionsImpl>(
      "", std::vector<std::string>{}, std::vector<std::string>{},
      std::vector<std::string>{"foo", "test"});
  testUtilV2(test_options.setExpectedALPNProtocol("test").setTransportSocketOptions(
      transport_socket_options));

  // Update the client TLS config to specify ALPN. The fallback value should no longer be used.
  // Note that the server prefers "test" over "bar", but since the client only configures "bar",
  // the resulting ALPN will be "bar" even though "test" is included in the fallback.
  server_ctx->add_alpn_protocols("bar");
  updateFilterChain(tls_context, *filter_chain);
  client.mutable_common_tls_context()->add_alpn_protocols("bar");
  testUtilV2(test_options.setExpectedALPNProtocol("bar").setTransportSocketOptions(
      transport_socket_options));
}