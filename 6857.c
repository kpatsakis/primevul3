void testSupportForStatelessSessionResumption(const std::string& server_ctx_yaml,
                                              const std::string& client_ctx_yaml,
                                              bool expect_support,
                                              const Network::Address::IpVersion ip_version) {
  Event::SimulatedTimeSystem time_system;
  ContextManagerImpl manager(*time_system);

  Stats::IsolatedStoreImpl server_stats_store;
  Api::ApiPtr server_api = Api::createApiForTest(server_stats_store, time_system);
  NiceMock<Runtime::MockLoader> runtime;
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      server_factory_context;
  ON_CALL(server_factory_context, api()).WillByDefault(ReturnRef(*server_api));

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext server_tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(server_ctx_yaml), server_tls_context);
  auto server_cfg =
      std::make_unique<ServerContextConfigImpl>(server_tls_context, server_factory_context);

  ServerSslSocketFactory server_ssl_socket_factory(std::move(server_cfg), manager,
                                                   server_stats_store, {});
  auto tcp_socket = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(ip_version));
  NiceMock<Network::MockTcpListenerCallbacks> callbacks;
  Event::DispatcherPtr dispatcher(server_api->allocateDispatcher("test_thread"));
  Network::ListenerPtr listener =
      dispatcher->createListener(tcp_socket, callbacks, runtime, true, false);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client_tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(client_ctx_yaml), client_tls_context);

  Stats::IsolatedStoreImpl client_stats_store;
  Api::ApiPtr client_api = Api::createApiForTest(client_stats_store, time_system);
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      client_factory_context;
  ON_CALL(client_factory_context, api()).WillByDefault(ReturnRef(*client_api));

  auto client_cfg =
      std::make_unique<ClientContextConfigImpl>(client_tls_context, client_factory_context);
  ClientSslSocketFactory ssl_socket_factory(std::move(client_cfg), manager, client_stats_store);
  Network::ClientConnectionPtr client_connection = dispatcher->createClientConnection(
      tcp_socket->connectionInfoProvider().localAddress(),
      Network::Address::InstanceConstSharedPtr(), ssl_socket_factory.createTransportSocket(nullptr),
      nullptr);

  Network::MockConnectionCallbacks client_connection_callbacks;
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  client_connection->connect();

  StreamInfo::StreamInfoImpl stream_info(time_system, nullptr);
  Network::ConnectionPtr server_connection;
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        server_connection = dispatcher->createServerConnection(
            std::move(socket), server_ssl_socket_factory.createTransportSocket(nullptr),
            stream_info);

        const SslHandshakerImpl* ssl_socket =
            dynamic_cast<const SslHandshakerImpl*>(server_connection->ssl().get());
        SSL* server_ssl_socket = ssl_socket->ssl();
        SSL_CTX* server_ssl_context = SSL_get_SSL_CTX(server_ssl_socket);
        if (expect_support) {
          EXPECT_EQ(0, (SSL_CTX_get_options(server_ssl_context) & SSL_OP_NO_TICKET));
        } else {
          EXPECT_EQ(SSL_OP_NO_TICKET, (SSL_CTX_get_options(server_ssl_context) & SSL_OP_NO_TICKET));
        }
      }));

  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void {
        client_connection->close(Network::ConnectionCloseType::NoFlush);
        server_connection->close(Network::ConnectionCloseType::NoFlush);
        dispatcher->exit();
      }));

  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
  dispatcher->run(Event::Dispatcher::RunType::Block);
}