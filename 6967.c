TEST_P(SslSocketTest, TestTransportSocketCallback) {
  // Make MockTransportSocketCallbacks.
  Network::MockIoHandle io_handle;
  NiceMock<Network::MockTransportSocketCallbacks> callbacks;
  ON_CALL(callbacks, ioHandle()).WillByDefault(ReturnRef(io_handle));

  // Make SslSocket.
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext> factory_context;
  Stats::TestUtil::TestStore stats_store;
  ON_CALL(factory_context, stats()).WillByDefault(ReturnRef(stats_store));
  NiceMock<LocalInfo::MockLocalInfo> local_info;
  ON_CALL(factory_context, localInfo()).WillByDefault(ReturnRef(local_info));

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext tls_context;
  auto client_cfg = std::make_unique<ClientContextConfigImpl>(tls_context, factory_context);

  ContextManagerImpl manager(time_system_);
  ClientSslSocketFactory client_ssl_socket_factory(std::move(client_cfg), manager, stats_store);

  Network::TransportSocketPtr transport_socket =
      client_ssl_socket_factory.createTransportSocket(nullptr);

  SslSocket* ssl_socket = dynamic_cast<SslSocket*>(transport_socket.get());

  // If no transport socket callbacks have been set, this method should return nullptr.
  EXPECT_EQ(ssl_socket->transportSocketCallbacks(), nullptr);

  // Otherwise, it should return a pointer to the set callbacks object.
  ssl_socket->setTransportSocketCallbacks(callbacks);
  EXPECT_EQ(ssl_socket->transportSocketCallbacks(), &callbacks);
}