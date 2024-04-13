TEST_P(SslSocketTest, ShutdownWithCloseNotify) {
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
  Network::MockTcpListenerCallbacks listener_callbacks;
  Network::ListenerPtr listener =
      dispatcher_->createListener(socket, listener_callbacks, runtime_, true, false);
  std::shared_ptr<Network::MockReadFilter> server_read_filter(new Network::MockReadFilter());
  std::shared_ptr<Network::MockReadFilter> client_read_filter(new Network::MockReadFilter());

  const std::string client_ctx_yaml = R"EOF(
    common_tls_context:
  )EOF";

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(client_ctx_yaml), tls_context);
  auto client_cfg = std::make_unique<ClientContextConfigImpl>(tls_context, factory_context_);
  Stats::TestUtil::TestStore client_stats_store;
  ClientSslSocketFactory client_ssl_socket_factory(std::move(client_cfg), manager,
                                                   client_stats_store);
  Network::ClientConnectionPtr client_connection = dispatcher_->createClientConnection(
      socket->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      client_ssl_socket_factory.createTransportSocket(nullptr), nullptr);
  Network::MockConnectionCallbacks client_connection_callbacks;
  client_connection->enableHalfClose(true);
  client_connection->addReadFilter(client_read_filter);
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  client_connection->connect();

  Network::ConnectionPtr server_connection;
  Network::MockConnectionCallbacks server_connection_callbacks;
  EXPECT_CALL(listener_callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        server_connection = dispatcher_->createServerConnection(
            std::move(socket), server_ssl_socket_factory.createTransportSocket(nullptr),
            stream_info_);
        server_connection->enableHalfClose(true);
        server_connection->addReadFilter(server_read_filter);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));
  EXPECT_CALL(*server_read_filter, onNewConnection());
  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void {
        Buffer::OwnedImpl data("hello");
        server_connection->write(data, true);
        EXPECT_EQ(data.length(), 0);
      }));

  EXPECT_CALL(*client_read_filter, onNewConnection())
      .WillOnce(Return(Network::FilterStatus::Continue));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected));
  EXPECT_CALL(*client_read_filter, onData(BufferStringEqual("hello"), true))
      .WillOnce(Invoke([&](Buffer::Instance& read_buffer, bool) -> Network::FilterStatus {
        read_buffer.drain(read_buffer.length());
        client_connection->close(Network::ConnectionCloseType::NoFlush);
        return Network::FilterStatus::StopIteration;
      }));
  EXPECT_CALL(*server_read_filter, onData(_, true));

  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void {
        server_connection->close(Network::ConnectionCloseType::NoFlush);
        dispatcher_->exit();
      }));

  dispatcher_->run(Event::Dispatcher::RunType::Block);
}