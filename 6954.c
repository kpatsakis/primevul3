TEST_P(SslSocketTest, DownstreamNotReadySslSocket) {
  Stats::TestUtil::TestStore stats_store;
  NiceMock<LocalInfo::MockLocalInfo> local_info;
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext> factory_context;
  NiceMock<Init::MockManager> init_manager;
  NiceMock<Event::MockDispatcher> dispatcher;
  EXPECT_CALL(factory_context, mainThreadDispatcher()).WillRepeatedly(ReturnRef(dispatcher));
  EXPECT_CALL(factory_context, localInfo()).WillOnce(ReturnRef(local_info));
  EXPECT_CALL(factory_context, stats()).WillOnce(ReturnRef(stats_store));
  EXPECT_CALL(factory_context, initManager()).WillRepeatedly(ReturnRef(init_manager));

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext tls_context;
  auto sds_secret_configs =
      tls_context.mutable_common_tls_context()->mutable_tls_certificate_sds_secret_configs()->Add();
  sds_secret_configs->set_name("abc.com");
  sds_secret_configs->mutable_sds_config();
  auto server_cfg = std::make_unique<ServerContextConfigImpl>(tls_context, factory_context);
  EXPECT_TRUE(server_cfg->tlsCertificates().empty());
  EXPECT_FALSE(server_cfg->isReady());

  ContextManagerImpl manager(time_system_);
  ServerSslSocketFactory server_ssl_socket_factory(std::move(server_cfg), manager, stats_store,
                                                   std::vector<std::string>{});
  auto transport_socket = server_ssl_socket_factory.createTransportSocket(nullptr);
  EXPECT_FALSE(transport_socket->startSecureTransport());                                  // Noop
  transport_socket->configureInitialCongestionWindow(200, std::chrono::microseconds(223)); // Noop
  EXPECT_EQ(EMPTY_STRING, transport_socket->protocol());
  EXPECT_EQ(nullptr, transport_socket->ssl());
  EXPECT_EQ(true, transport_socket->canFlushClose());
  Buffer::OwnedImpl buffer;
  Network::IoResult result = transport_socket->doRead(buffer);
  EXPECT_EQ(Network::PostIoAction::Close, result.action_);
  result = transport_socket->doWrite(buffer, true);
  EXPECT_EQ(Network::PostIoAction::Close, result.action_);
  EXPECT_EQ("TLS error: Secret is not supplied by SDS", transport_socket->failureReason());
}