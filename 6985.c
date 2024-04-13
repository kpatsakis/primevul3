  TestUtilOptionsV2(
      const envoy::config::listener::v3::Listener& listener,
      const envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext& client_ctx_proto,
      bool expect_success, Network::Address::IpVersion version)
      : TestUtilOptionsBase(expect_success, version), listener_(listener),
        client_ctx_proto_(client_ctx_proto), transport_socket_options_(nullptr) {
    if (expect_success) {
      setExpectedServerStats("ssl.handshake").setExpectedClientStats("ssl.handshake");
    } else {
      setExpectedServerStats("ssl.fail_verify_error")
          .setExpectedClientStats("ssl.connection_error");
    }
  }