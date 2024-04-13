TestUtilOptionsV2 createProtocolTestOptions(
    const envoy::config::listener::v3::Listener& listener,
    const envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext& client_ctx,
    Network::Address::IpVersion version, std::string protocol) {
  std::string stats = "ssl.versions." + protocol;
  TestUtilOptionsV2 options(listener, client_ctx, true, version);
  options.setExpectedServerStats(stats).setExpectedClientStats(stats);
  return options.setExpectedProtocolVersion(protocol);
}