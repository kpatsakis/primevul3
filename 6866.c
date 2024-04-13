  TestUtilOptions(const std::string& client_ctx_yaml, const std::string& server_ctx_yaml,
                  bool expect_success, Network::Address::IpVersion version)
      : TestUtilOptionsBase(expect_success, version), client_ctx_yaml_(client_ctx_yaml),
        server_ctx_yaml_(server_ctx_yaml), expect_no_cert_(false), expect_no_cert_chain_(false),
        expect_private_key_method_(false),
        expected_server_close_event_(Network::ConnectionEvent::RemoteClose) {
    if (expect_success) {
      setExpectedServerStats("ssl.handshake");
    } else {
      setExpectedServerStats("ssl.fail_verify_error");
    }
  }