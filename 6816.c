void testUtilV2(const TestUtilOptionsV2& options) {
  Event::SimulatedTimeSystem time_system;
  ContextManagerImpl manager(*time_system);

  // SNI-based selection logic isn't happening in SslSocket anymore.
  ASSERT(options.listener().filter_chains().size() == 1);
  const auto& filter_chain = options.listener().filter_chains(0);
  std::vector<std::string> server_names(filter_chain.filter_chain_match().server_names().begin(),
                                        filter_chain.filter_chain_match().server_names().end());
  Stats::TestUtil::TestStore server_stats_store;
  Api::ApiPtr server_api = Api::createApiForTest(server_stats_store, time_system);
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      server_factory_context;
  NiceMock<Runtime::MockLoader> runtime;
  ON_CALL(server_factory_context, api()).WillByDefault(ReturnRef(*server_api));

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  const envoy::config::core::v3::TransportSocket& transport_socket =
      filter_chain.transport_socket();
  ASSERT(transport_socket.has_typed_config());
  transport_socket.typed_config().UnpackTo(&tls_context);

  auto server_cfg = std::make_unique<ServerContextConfigImpl>(tls_context, server_factory_context);

  ServerSslSocketFactory server_ssl_socket_factory(std::move(server_cfg), manager,
                                                   server_stats_store, server_names);
  EXPECT_FALSE(server_ssl_socket_factory.usesProxyProtocolOptions());

  Event::DispatcherPtr dispatcher(server_api->allocateDispatcher("test_thread"));
  auto socket = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(options.version()));
  NiceMock<Network::MockTcpListenerCallbacks> callbacks;
  Network::ListenerPtr listener =
      dispatcher->createListener(socket, callbacks, runtime, true, false);

  Stats::TestUtil::TestStore client_stats_store;
  Api::ApiPtr client_api = Api::createApiForTest(client_stats_store, time_system);
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      client_factory_context;
  ON_CALL(client_factory_context, api()).WillByDefault(ReturnRef(*client_api));

  auto client_cfg =
      std::make_unique<ClientContextConfigImpl>(options.clientCtxProto(), client_factory_context);
  ClientSslSocketFactory client_ssl_socket_factory(std::move(client_cfg), manager,
                                                   client_stats_store);
  Network::ClientConnectionPtr client_connection = dispatcher->createClientConnection(
      socket->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      client_ssl_socket_factory.createTransportSocket(options.transportSocketOptions()), nullptr);

  if (!options.clientSession().empty()) {
    const SslHandshakerImpl* ssl_socket =
        dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
    SSL* client_ssl_socket = ssl_socket->ssl();
    SSL_CTX* client_ssl_context = SSL_get_SSL_CTX(client_ssl_socket);
    SSL_SESSION* client_ssl_session =
        SSL_SESSION_from_bytes(reinterpret_cast<const uint8_t*>(options.clientSession().data()),
                               options.clientSession().size(), client_ssl_context);
    int rc = SSL_set_session(client_ssl_socket, client_ssl_session);
    ASSERT(rc == 1);
    SSL_SESSION_free(client_ssl_session);
  }

  Network::ConnectionPtr server_connection;
  Network::MockConnectionCallbacks server_connection_callbacks;
  NiceMock<StreamInfo::MockStreamInfo> stream_info;
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        std::string sni = options.transportSocketOptions() != nullptr &&
                                  options.transportSocketOptions()->serverNameOverride().has_value()
                              ? options.transportSocketOptions()->serverNameOverride().value()
                              : options.clientCtxProto().sni();
        socket->setRequestedServerName(sni);
        Network::TransportSocketPtr transport_socket =
            server_ssl_socket_factory.createTransportSocket(nullptr);
        EXPECT_FALSE(transport_socket->startSecureTransport());
        server_connection = dispatcher->createServerConnection(
            std::move(socket), std::move(transport_socket), stream_info);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));

  Network::MockConnectionCallbacks client_connection_callbacks;
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  client_connection->connect();

  size_t connect_count = 0;
  auto connect_second_time = [&]() {
    if (++connect_count == 2) {
      if (!options.expectedServerCertDigest().empty()) {
        EXPECT_EQ(options.expectedServerCertDigest(),
                  client_connection->ssl()->sha256PeerCertificateDigest());
      }
      if (!options.expectedALPNProtocol().empty()) {
        EXPECT_EQ(options.expectedALPNProtocol(), client_connection->nextProtocol());
      }
      EXPECT_EQ(options.expectedClientCertUri(), server_connection->ssl()->uriSanPeerCertificate());
      const SslHandshakerImpl* ssl_socket =
          dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
      SSL* client_ssl_socket = ssl_socket->ssl();
      if (!options.expectedProtocolVersion().empty()) {
        // Assert twice to ensure a cached value is returned and still valid.
        EXPECT_EQ(options.expectedProtocolVersion(), client_connection->ssl()->tlsVersion());
        EXPECT_EQ(options.expectedProtocolVersion(), client_connection->ssl()->tlsVersion());
      }
      if (!options.expectedCiphersuite().empty()) {
        EXPECT_EQ(options.expectedCiphersuite(), client_connection->ssl()->ciphersuiteString());
        const SSL_CIPHER* cipher =
            SSL_get_cipher_by_value(client_connection->ssl()->ciphersuiteId());
        EXPECT_NE(nullptr, cipher);
        EXPECT_EQ(options.expectedCiphersuite(), SSL_CIPHER_get_name(cipher));
      }

      absl::optional<std::string> server_ssl_requested_server_name;
      const SslHandshakerImpl* server_ssl_socket =
          dynamic_cast<const SslHandshakerImpl*>(server_connection->ssl().get());
      SSL* server_ssl = server_ssl_socket->ssl();
      auto requested_server_name = SSL_get_servername(server_ssl, TLSEXT_NAMETYPE_host_name);
      if (requested_server_name != nullptr) {
        server_ssl_requested_server_name = std::string(requested_server_name);
      }

      if (!options.expectedRequestedServerName().empty()) {
        EXPECT_TRUE(server_ssl_requested_server_name.has_value());
        EXPECT_EQ(options.expectedRequestedServerName(), server_ssl_requested_server_name.value());
      } else {
        EXPECT_FALSE(server_ssl_requested_server_name.has_value());
      }

      const uint16_t tls_version = SSL_version(client_ssl_socket);
      if (SSL3_VERSION <= tls_version && tls_version <= TLS1_2_VERSION) {
        // Prior to TLS 1.3, one should be able to resume the session. With TLS
        // 1.3, tickets come after the handshake and the SSL_SESSION on the
        // client is a dummy object.
        SSL_SESSION* client_ssl_session = SSL_get_session(client_ssl_socket);
        EXPECT_TRUE(SSL_SESSION_is_resumable(client_ssl_session));
      }
      server_connection->close(Network::ConnectionCloseType::NoFlush);
      client_connection->close(Network::ConnectionCloseType::NoFlush);
      dispatcher->exit();
    }
  };

  size_t close_count = 0;
  auto close_second_time = [&close_count, &dispatcher]() {
    if (++close_count == 2) {
      dispatcher->exit();
    }
  };

  if (options.expectSuccess()) {
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
    EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void {
          EXPECT_EQ(options.expectedRequestedServerName(),
                    server_connection->requestedServerName());
          connect_second_time();
        }));
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
    EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
  } else {
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { close_second_time(); }));
    EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { close_second_time(); }));
  }

  dispatcher->run(Event::Dispatcher::RunType::Block);

  if (!options.expectedServerStats().empty()) {
    EXPECT_EQ(1UL, server_stats_store.counter(options.expectedServerStats()).value())
        << options.expectedServerStats();
  }

  if (!options.expectedClientStats().empty()) {
    EXPECT_EQ(1UL, client_stats_store.counter(options.expectedClientStats()).value());
  }

  if (options.expectSuccess()) {
    EXPECT_EQ("", client_connection->transportFailureReason());
    EXPECT_EQ("", server_connection->transportFailureReason());
  } else {
    EXPECT_THAT(std::string(client_connection->transportFailureReason()),
                ContainsRegex(options.expectedTransportFailureReasonContains()));
    EXPECT_NE("", server_connection->transportFailureReason());
  }
}