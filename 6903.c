TEST_P(SslReadBufferLimitTest, TestBind) {
  std::string address_string = TestUtility::getIpv4Loopback();
  if (GetParam() == Network::Address::IpVersion::v4) {
    source_address_ = Network::Address::InstanceConstSharedPtr{
        new Network::Address::Ipv4Instance(address_string, 0, nullptr)};
  } else {
    address_string = "::1";
    source_address_ = Network::Address::InstanceConstSharedPtr{
        new Network::Address::Ipv6Instance(address_string, 0, nullptr)};
  }

  initialize();

  EXPECT_CALL(listener_callbacks_, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        server_connection_ = dispatcher_->createServerConnection(
            std::move(socket), server_ssl_socket_factory_->createTransportSocket(nullptr),
            stream_info_);
        server_connection_->addConnectionCallbacks(server_callbacks_);
        server_connection_->addReadFilter(read_filter_);
        EXPECT_EQ("", server_connection_->nextProtocol());
      }));

  EXPECT_CALL(client_callbacks_, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { dispatcher_->exit(); }));
  dispatcher_->run(Event::Dispatcher::RunType::Block);

  EXPECT_EQ(address_string,
            server_connection_->connectionInfoProvider().remoteAddress()->ip()->addressAsString());

  disconnect();
}