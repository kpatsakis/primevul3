TEST_P(SslSocketTest, ClientAuthCrossListenerSessionResumption) {
  const std::string server_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ca_cert.pem"
  require_client_certificate: true
)EOF";

  const std::string server2_ctx_yaml = R"EOF(
  common_tls_context:
    tls_certificates:
      certificate_chain:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_cert.pem"
      private_key:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/unittest_key.pem"
    validation_context:
      trusted_ca:
        filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/fake_ca_cert.pem"
  require_client_certificate: true
  session_ticket_keys:
    keys:
      filename: "{{ test_rundir }}/test/extensions/transport_sockets/tls/test_data/ticket_key_a"
)EOF";

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context1;
  TestUtility::loadFromYaml(TestEnvironment::substitute(server_ctx_yaml), tls_context1);
  auto server_cfg = std::make_unique<ServerContextConfigImpl>(tls_context1, factory_context_);
  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context2;
  TestUtility::loadFromYaml(TestEnvironment::substitute(server2_ctx_yaml), tls_context2);
  auto server2_cfg = std::make_unique<ServerContextConfigImpl>(tls_context2, factory_context_);
  ContextManagerImpl manager(time_system_);
  Stats::TestUtil::TestStore server_stats_store;
  ServerSslSocketFactory server_ssl_socket_factory(std::move(server_cfg), manager,
                                                   server_stats_store, std::vector<std::string>{});
  ServerSslSocketFactory server2_ssl_socket_factory(std::move(server2_cfg), manager,
                                                    server_stats_store, std::vector<std::string>{});

  auto socket = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(GetParam()));
  auto socket2 = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(GetParam()));
  Network::MockTcpListenerCallbacks callbacks;
  Network::ListenerPtr listener =
      dispatcher_->createListener(socket, callbacks, runtime_, true, false);
  Network::ListenerPtr listener2 =
      dispatcher_->createListener(socket2, callbacks, runtime_, true, false);
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

  Network::MockConnectionCallbacks client_connection_callbacks;
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  client_connection->connect();

  SSL_SESSION* ssl_session = nullptr;
  Network::ConnectionPtr server_connection;
  Network::MockConnectionCallbacks server_connection_callbacks;
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& accepted_socket) -> void {
        Network::TransportSocketFactory& tsf =
            accepted_socket->connectionInfoProvider().localAddress() ==
                    socket->connectionInfoProvider().localAddress()
                ? server_ssl_socket_factory
                : server2_ssl_socket_factory;
        server_connection = dispatcher_->createServerConnection(
            std::move(accepted_socket), tsf.createTransportSocket(nullptr), stream_info_);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));

  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected));
  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void {
        const SslHandshakerImpl* ssl_socket =
            dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
        ssl_session = SSL_get1_session(ssl_socket->ssl());
        EXPECT_TRUE(SSL_SESSION_is_resumable(ssl_session));
        server_connection->close(Network::ConnectionCloseType::NoFlush);
        client_connection->close(Network::ConnectionCloseType::NoFlush);
        dispatcher_->exit();
      }));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));

  dispatcher_->run(Event::Dispatcher::RunType::Block);

  EXPECT_EQ(1UL, server_stats_store.counter("ssl.handshake").value());
  EXPECT_EQ(1UL, client_stats_store.counter("ssl.handshake").value());

  client_connection = dispatcher_->createClientConnection(
      socket2->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      ssl_socket_factory.createTransportSocket(nullptr), nullptr);
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  const SslHandshakerImpl* ssl_socket =
      dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
  SSL_set_session(ssl_socket->ssl(), ssl_session);
  SSL_SESSION_free(ssl_session);

  client_connection->connect();

  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& accepted_socket) -> void {
        Network::TransportSocketFactory& tsf =
            accepted_socket->connectionInfoProvider().localAddress() ==
                    socket->connectionInfoProvider().localAddress()
                ? server_ssl_socket_factory
                : server2_ssl_socket_factory;
        server_connection = dispatcher_->createServerConnection(
            std::move(accepted_socket), tsf.createTransportSocket(nullptr), stream_info_);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));
  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { dispatcher_->exit(); }));

  dispatcher_->run(Event::Dispatcher::RunType::Block);

  EXPECT_EQ(1UL, client_stats_store.counter("ssl.connection_error").value());
  EXPECT_EQ(0UL, server_stats_store.counter("ssl.session_reused").value());
  EXPECT_EQ(0UL, client_stats_store.counter("ssl.session_reused").value());
}