TEST_P(SslSocketTest, ClientAuthMultipleCAs) {
  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_certificates.pem"
)EOF";

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext server_tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(server_ctx_yaml), server_tls_context);
  auto server_cfg = std::make_unique<ServerContextConfigImpl>(server_tls_context, factory_context_);
  ContextManagerImpl manager(time_system_);
  Stats::TestUtil::TestStore server_stats_store;
  ServerSslSocketFactory server_ssl_socket_factory(std::move(server_cfg), manager,
                                                   server_stats_store, std::vector<std::string>{});

  auto socket = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(GetParam()));
  Network::MockTcpListenerCallbacks callbacks;
  Network::ListenerPtr listener =
      dispatcher_->createListener(socket, callbacks, runtime_, true, false);

  const std::string client_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/no_san_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/no_san_key.pem"
)EOF";

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(client_ctx_yaml), tls_context);
  auto client_cfg = std::make_unique<ClientContextConfigImpl>(tls_context, factory_context_);
  Stats::TestUtil::TestStore client_stats_store;
  ClientSslSocketFactory ssl_socket_factory(std::move(client_cfg), manager, client_stats_store);
  Network::ClientConnectionPtr client_connection = dispatcher_->createClientConnection(
      socket->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      ssl_socket_factory.createTransportSocket(nullptr), nullptr);

  // Verify that server sent list with 2 acceptable client certificate CA names.
  const SslHandshakerImpl* ssl_socket =
      dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
  SSL_set_cert_cb(
      ssl_socket->ssl(),
      [](SSL* ssl, void*) -> int {
        STACK_OF(X509_NAME)* list = SSL_get_client_CA_list(ssl);
        EXPECT_NE(nullptr, list);
        EXPECT_EQ(2U, sk_X509_NAME_num(list));
        return 1;
      },
      nullptr);

  client_connection->connect();

  Network::ConnectionPtr server_connection;
  Network::MockConnectionCallbacks server_connection_callbacks;
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        server_connection = dispatcher_->createServerConnection(
            std::move(socket), server_ssl_socket_factory.createTransportSocket(nullptr),
            stream_info_);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));

  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void {
        server_connection->close(Network::ConnectionCloseType::NoFlush);
        client_connection->close(Network::ConnectionCloseType::NoFlush);
        dispatcher_->exit();
      }));
  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));

  dispatcher_->run(Event::Dispatcher::RunType::Block);

  EXPECT_EQ(1UL, server_stats_store.counter("ssl.handshake").value());
}