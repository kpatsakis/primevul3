void SslSocketTest::testClientSessionResumption(const std::string& server_ctx_yaml,
                                                const std::string& client_ctx_yaml,
                                                bool expect_reuse,
                                                const Network::Address::IpVersion version) {
  InSequence s;

  ContextManagerImpl manager(time_system_);

  Stats::TestUtil::TestStore server_stats_store;
  Api::ApiPtr server_api = Api::createApiForTest(server_stats_store, time_system_);
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      server_factory_context;
  ON_CALL(server_factory_context, api()).WillByDefault(ReturnRef(*server_api));

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext server_ctx_proto;
  TestUtility::loadFromYaml(TestEnvironment::substitute(server_ctx_yaml), server_ctx_proto);
  auto server_cfg =
      std::make_unique<ServerContextConfigImpl>(server_ctx_proto, server_factory_context);
  ServerSslSocketFactory server_ssl_socket_factory(std::move(server_cfg), manager,
                                                   server_stats_store, std::vector<std::string>{});

  auto socket = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(version));
  NiceMock<Network::MockTcpListenerCallbacks> callbacks;
  Api::ApiPtr api = Api::createApiForTest(server_stats_store, time_system_);
  Event::DispatcherPtr dispatcher(server_api->allocateDispatcher("test_thread"));
  Network::ListenerPtr listener =
      dispatcher->createListener(socket, callbacks, runtime_, true, false);

  Network::ConnectionPtr server_connection;
  Network::MockConnectionCallbacks server_connection_callbacks;

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client_ctx_proto;
  TestUtility::loadFromYaml(TestEnvironment::substitute(client_ctx_yaml), client_ctx_proto);

  Stats::TestUtil::TestStore client_stats_store;
  Api::ApiPtr client_api = Api::createApiForTest(client_stats_store, time_system_);
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      client_factory_context;
  ON_CALL(client_factory_context, api()).WillByDefault(ReturnRef(*client_api));

  auto client_cfg =
      std::make_unique<ClientContextConfigImpl>(client_ctx_proto, client_factory_context);
  ClientSslSocketFactory client_ssl_socket_factory(std::move(client_cfg), manager,
                                                   client_stats_store);
  Network::ClientConnectionPtr client_connection = dispatcher->createClientConnection(
      socket->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      client_ssl_socket_factory.createTransportSocket(nullptr), nullptr);

  Network::MockConnectionCallbacks client_connection_callbacks;
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  client_connection->connect();

  size_t connect_count = 0;
  auto connect_second_time = [&connect_count, &server_connection]() {
    if (++connect_count == 2) {
      server_connection->close(Network::ConnectionCloseType::NoFlush);
    }
  };

  size_t close_count = 0;
  auto close_second_time = [&close_count, &dispatcher]() {
    if (++close_count == 2) {
      dispatcher->exit();
    }
  };

  // WillRepeatedly doesn't work with InSequence.
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        server_connection = dispatcher->createServerConnection(
            std::move(socket), server_ssl_socket_factory.createTransportSocket(nullptr),
            stream_info_);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));

  const bool expect_tls13 =
      client_ctx_proto.common_tls_context().tls_params().tls_maximum_protocol_version() ==
          envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3 &&
      server_ctx_proto.common_tls_context().tls_params().tls_maximum_protocol_version() ==
          envoy::extensions::transport_sockets::tls::v3::TlsParameters::TLSv1_3;

  // The order of "Connected" events depends on the version of the TLS protocol (1.3 or older).
  if (expect_tls13) {
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
    EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
  } else {
    EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
  }
  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { close_second_time(); }));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { close_second_time(); }));

  dispatcher->run(Event::Dispatcher::RunType::Block);

  EXPECT_EQ(0UL, server_stats_store.counter("ssl.session_reused").value());
  EXPECT_EQ(0UL, client_stats_store.counter("ssl.session_reused").value());

  connect_count = 0;
  close_count = 0;

  client_connection = dispatcher->createClientConnection(
      socket->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      client_ssl_socket_factory.createTransportSocket(nullptr), nullptr);
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  client_connection->connect();

  // WillRepeatedly doesn't work with InSequence.
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        server_connection = dispatcher->createServerConnection(
            std::move(socket), server_ssl_socket_factory.createTransportSocket(nullptr),
            stream_info_);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));

  // The order of "Connected" events depends on the version of the TLS protocol (1.3 or older),
  // and whether or not the session was successfully resumed.
  if (expect_tls13 || expect_reuse) {
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
    EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
  } else {
    EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
  }
  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { close_second_time(); }));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { close_second_time(); }));

  dispatcher->run(Event::Dispatcher::RunType::Block);

  EXPECT_EQ(expect_reuse ? 1UL : 0UL, server_stats_store.counter("ssl.session_reused").value());
  EXPECT_EQ(expect_reuse ? 1UL : 0UL, client_stats_store.counter("ssl.session_reused").value());
}