void testTicketSessionResumption(const std::string& server_ctx_yaml1,
                                 const std::vector<std::string>& server_names1,
                                 const std::string& server_ctx_yaml2,
                                 const std::vector<std::string>& server_names2,
                                 const std::string& client_ctx_yaml, bool expect_reuse,
                                 const Network::Address::IpVersion ip_version,
                                 const uint32_t expected_lifetime_hint = 0) {
  Event::SimulatedTimeSystem time_system;
  ContextManagerImpl manager(*time_system);

  Stats::TestUtil::TestStore server_stats_store;
  Api::ApiPtr server_api = Api::createApiForTest(server_stats_store, time_system);
  NiceMock<Runtime::MockLoader> runtime;
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      server_factory_context;
  ON_CALL(server_factory_context, api()).WillByDefault(ReturnRef(*server_api));

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext server_tls_context1;
  TestUtility::loadFromYaml(TestEnvironment::substitute(server_ctx_yaml1), server_tls_context1);
  auto server_cfg1 =
      std::make_unique<ServerContextConfigImpl>(server_tls_context1, server_factory_context);

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext server_tls_context2;
  TestUtility::loadFromYaml(TestEnvironment::substitute(server_ctx_yaml2), server_tls_context2);
  auto server_cfg2 =
      std::make_unique<ServerContextConfigImpl>(server_tls_context2, server_factory_context);
  ServerSslSocketFactory server_ssl_socket_factory1(std::move(server_cfg1), manager,
                                                    server_stats_store, server_names1);
  ServerSslSocketFactory server_ssl_socket_factory2(std::move(server_cfg2), manager,
                                                    server_stats_store, server_names2);

  auto socket1 = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(ip_version));
  auto socket2 = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(ip_version));
  NiceMock<Network::MockTcpListenerCallbacks> callbacks;
  Event::DispatcherPtr dispatcher(server_api->allocateDispatcher("test_thread"));
  Network::ListenerPtr listener1 =
      dispatcher->createListener(socket1, callbacks, runtime, true, false);
  Network::ListenerPtr listener2 =
      dispatcher->createListener(socket2, callbacks, runtime, true, false);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client_tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(client_ctx_yaml), client_tls_context);

  Stats::TestUtil::TestStore client_stats_store;
  Api::ApiPtr client_api = Api::createApiForTest(client_stats_store, time_system);
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      client_factory_context;
  ON_CALL(client_factory_context, api()).WillByDefault(ReturnRef(*client_api));

  auto client_cfg =
      std::make_unique<ClientContextConfigImpl>(client_tls_context, client_factory_context);
  ClientSslSocketFactory ssl_socket_factory(std::move(client_cfg), manager, client_stats_store);
  Network::ClientConnectionPtr client_connection = dispatcher->createClientConnection(
      socket1->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      ssl_socket_factory.createTransportSocket(nullptr), nullptr);

  Network::MockConnectionCallbacks client_connection_callbacks;
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  client_connection->connect();

  SSL_SESSION* ssl_session = nullptr;
  Network::ConnectionPtr server_connection;
  StreamInfo::StreamInfoImpl stream_info(time_system, nullptr);
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        Network::TransportSocketFactory& tsf =
            socket->connectionInfoProvider().localAddress() ==
                    socket1->connectionInfoProvider().localAddress()
                ? server_ssl_socket_factory1
                : server_ssl_socket_factory2;
        server_connection = dispatcher->createServerConnection(
            std::move(socket), tsf.createTransportSocket(nullptr), stream_info);
      }));

  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void {
        const SslHandshakerImpl* ssl_socket =
            dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
        ssl_session = SSL_get1_session(ssl_socket->ssl());
        EXPECT_TRUE(SSL_SESSION_is_resumable(ssl_session));
        if (expected_lifetime_hint) {
          auto lifetime_hint = SSL_SESSION_get_ticket_lifetime_hint(ssl_session);
          EXPECT_TRUE(lifetime_hint <= expected_lifetime_hint);
        }
        client_connection->close(Network::ConnectionCloseType::NoFlush);
        server_connection->close(Network::ConnectionCloseType::NoFlush);
        dispatcher->exit();
      }));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));

  dispatcher->run(Event::Dispatcher::RunType::Block);

  EXPECT_EQ(0UL, server_stats_store.counter("ssl.session_reused").value());
  EXPECT_EQ(0UL, client_stats_store.counter("ssl.session_reused").value());

  client_connection = dispatcher->createClientConnection(
      socket2->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      ssl_socket_factory.createTransportSocket(nullptr), nullptr);
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  const SslHandshakerImpl* ssl_socket =
      dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
  SSL_set_session(ssl_socket->ssl(), ssl_session);
  SSL_SESSION_free(ssl_session);

  client_connection->connect();

  Network::MockConnectionCallbacks server_connection_callbacks;
  StreamInfo::StreamInfoImpl stream_info2(time_system, nullptr);
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        Network::TransportSocketFactory& tsf =
            socket->connectionInfoProvider().localAddress() ==
                    socket1->connectionInfoProvider().localAddress()
                ? server_ssl_socket_factory1
                : server_ssl_socket_factory2;
        server_connection = dispatcher->createServerConnection(
            std::move(socket), tsf.createTransportSocket(nullptr), stream_info2);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));

  // Different tests have different order of whether client or server gets Connected event
  // first, so always wait until both have happened.
  size_t connect_count = 0;
  auto connect_second_time = [&connect_count, &dispatcher, &server_connection, &client_connection,
                              expect_reuse]() {
    connect_count++;
    if (connect_count == 2) {
      if (expect_reuse) {
        EXPECT_NE(EMPTY_STRING, server_connection->ssl()->sessionId());
        EXPECT_EQ(server_connection->ssl()->sessionId(), client_connection->ssl()->sessionId());
      } else {
        EXPECT_EQ(EMPTY_STRING, server_connection->ssl()->sessionId());
      }
      client_connection->close(Network::ConnectionCloseType::NoFlush);
      server_connection->close(Network::ConnectionCloseType::NoFlush);
      dispatcher->exit();
    }
  };

  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));

  dispatcher->run(Event::Dispatcher::RunType::Block);

  EXPECT_EQ(expect_reuse ? 1UL : 0UL, server_stats_store.counter("ssl.session_reused").value());
  EXPECT_EQ(expect_reuse ? 1UL : 0UL, client_stats_store.counter("ssl.session_reused").value());
}