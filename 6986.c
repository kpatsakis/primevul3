void updateFilterChain(
    const envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext& tls_context,
    envoy::config::listener::v3::FilterChain& filter_chain) {
  filter_chain.mutable_transport_socket()->mutable_typed_config()->PackFrom(tls_context);
}