TEST_P(SslSocketTest, GetRequestedServerName) {
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
  client.set_sni("lyft.com");

  TestUtilOptionsV2 test_options(listener, client, true, GetParam());
  testUtilV2(test_options.setExpectedRequestedServerName("lyft.com"));
}