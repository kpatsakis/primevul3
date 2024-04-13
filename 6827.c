TEST_P(SslSocketTest, FlushCloseDuringHandshake) {
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

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(server_ctx_yaml), tls_context);
  auto server_cfg = std::make_unique<ServerContextConfigImpl>(tls_context, factory_context_);
  ContextManagerImpl manager(time_system_);
  Stats::TestUtil::TestStore server_stats_store;
  ServerSslSocketFactory server_ssl_socket_factory(std::move(server_cfg), manager,
                                                   server_stats_store, std::vector<std::string>{});

  auto socket = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(GetParam()));
  Network::MockTcpListenerCallbacks callbacks;
  Network::ListenerPtr listener =
      dispatcher_->createListener(socket, callbacks, runtime_, true, false);

  Network::ClientConnectionPtr client_connection = dispatcher_->createClientConnection(
      socket->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      Network::Test::createRawBufferSocket(), nullptr);
  client_connection->connect();
  Network::MockConnectionCallbacks client_connection_callbacks;
  client_connection->addConnectionCallbacks(client_connection_callbacks);

  Network::ConnectionPtr server_connection;
  Network::MockConnectionCallbacks server_connection_callbacks;
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        server_connection = dispatcher_->createServerConnection(
            std::move(socket), server_ssl_socket_factory.createTransportSocket(nullptr),
            stream_info_);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
        Buffer::OwnedImpl data("hello");
        server_connection->write(data, false);
        server_connection->close(Network::ConnectionCloseType::FlushWrite);
      }));

  EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::Connected));
  EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { dispatcher_->exit(); }));

  dispatcher_->run(Event::Dispatcher::RunType::Block);
}