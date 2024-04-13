void testUtil(const TestUtilOptions& options) {
  Event::SimulatedTimeSystem time_system;

  Stats::TestUtil::TestStore server_stats_store;
  Api::ApiPtr server_api = Api::createApiForTest(server_stats_store, time_system);
  NiceMock<Runtime::MockLoader> runtime;
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      server_factory_context;
  ON_CALL(server_factory_context, api()).WillByDefault(ReturnRef(*server_api));

  // For private key method testing.
  NiceMock<Ssl::MockContextManager> context_manager;
  Extensions::PrivateKeyMethodProvider::TestPrivateKeyMethodFactory test_factory;
  Registry::InjectFactory<Ssl::PrivateKeyMethodProviderInstanceFactory>
      test_private_key_method_factory(test_factory);
  PrivateKeyMethodManagerImpl private_key_method_manager;
  if (options.expectedPrivateKeyMethod()) {
    EXPECT_CALL(server_factory_context, sslContextManager())
        .WillOnce(ReturnRef(context_manager))
        .WillRepeatedly(ReturnRef(context_manager));
    EXPECT_CALL(context_manager, privateKeyMethodManager())
        .WillOnce(ReturnRef(private_key_method_manager))
        .WillRepeatedly(ReturnRef(private_key_method_manager));
  }

  envoy::extensions::transport_sockets::tls::v3::DownstreamTlsContext server_tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(options.serverCtxYaml()),
                            server_tls_context);
  auto server_cfg =
      std::make_unique<ServerContextConfigImpl>(server_tls_context, server_factory_context);
  ContextManagerImpl manager(*time_system);
  ServerSslSocketFactory server_ssl_socket_factory(std::move(server_cfg), manager,
                                                   server_stats_store, std::vector<std::string>{});

  Event::DispatcherPtr dispatcher = server_api->allocateDispatcher("test_thread");
  auto socket = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
      Network::Test::getCanonicalLoopbackAddress(options.version()));
  Network::MockTcpListenerCallbacks callbacks;
  Network::ListenerPtr listener =
      dispatcher->createListener(socket, callbacks, runtime, true, false);

  envoy::extensions::transport_sockets::tls::v3::UpstreamTlsContext client_tls_context;
  TestUtility::loadFromYaml(TestEnvironment::substitute(options.clientCtxYaml()),
                            client_tls_context);

  Stats::TestUtil::TestStore client_stats_store;
  Api::ApiPtr client_api = Api::createApiForTest(client_stats_store, time_system);
  testing::NiceMock<Server::Configuration::MockTransportSocketFactoryContext>
      client_factory_context;
  ON_CALL(client_factory_context, api()).WillByDefault(ReturnRef(*client_api));

  auto client_cfg =
      std::make_unique<ClientContextConfigImpl>(client_tls_context, client_factory_context);
  ClientSslSocketFactory client_ssl_socket_factory(std::move(client_cfg), manager,
                                                   client_stats_store);
  Network::ClientConnectionPtr client_connection = dispatcher->createClientConnection(
      socket->connectionInfoProvider().localAddress(), Network::Address::InstanceConstSharedPtr(),
      client_ssl_socket_factory.createTransportSocket(nullptr), nullptr);
  Network::ConnectionPtr server_connection;
  Network::MockConnectionCallbacks server_connection_callbacks;
  NiceMock<StreamInfo::MockStreamInfo> stream_info;
  EXPECT_CALL(callbacks, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        auto ssl_socket = server_ssl_socket_factory.createTransportSocket(nullptr);
        // configureInitialCongestionWindow is an unimplemented empty function, this is just to
        // increase code coverage.
        ssl_socket->configureInitialCongestionWindow(100, std::chrono::microseconds(123));
        server_connection = dispatcher->createServerConnection(std::move(socket),
                                                               std::move(ssl_socket), stream_info);
        server_connection->addConnectionCallbacks(server_connection_callbacks);
      }));

  if (options.ocspStaplingEnabled()) {
    const SslHandshakerImpl* ssl_socket =
        dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
    SSL_enable_ocsp_stapling(ssl_socket->ssl());
  }

  Network::MockConnectionCallbacks client_connection_callbacks;
  client_connection->addConnectionCallbacks(client_connection_callbacks);
  client_connection->connect();

  size_t connect_count = 0;
  auto connect_second_time = [&]() {
    if (++connect_count == 2) {
      if (!options.expectedSha256Digest().empty()) {
        // Assert twice to ensure a cached value is returned and still valid.
        EXPECT_EQ(options.expectedSha256Digest(),
                  server_connection->ssl()->sha256PeerCertificateDigest());
        EXPECT_EQ(options.expectedSha256Digest(),
                  server_connection->ssl()->sha256PeerCertificateDigest());
      }
      if (!options.expectedSha1Digest().empty()) {
        // Assert twice to ensure a cached value is returned and still valid.
        EXPECT_EQ(options.expectedSha1Digest(),
                  server_connection->ssl()->sha1PeerCertificateDigest());
        EXPECT_EQ(options.expectedSha1Digest(),
                  server_connection->ssl()->sha1PeerCertificateDigest());
      }
      // Assert twice to ensure a cached value is returned and still valid.
      EXPECT_EQ(options.expectedClientCertUri(), server_connection->ssl()->uriSanPeerCertificate());
      EXPECT_EQ(options.expectedClientCertUri(), server_connection->ssl()->uriSanPeerCertificate());

      if (!options.expectedLocalUri().empty()) {
        // Assert twice to ensure a cached value is returned and still valid.
        EXPECT_EQ(options.expectedLocalUri(), server_connection->ssl()->uriSanLocalCertificate());
        EXPECT_EQ(options.expectedLocalUri(), server_connection->ssl()->uriSanLocalCertificate());
      }
      EXPECT_EQ(options.expectedSerialNumber(),
                server_connection->ssl()->serialNumberPeerCertificate());
      if (!options.expectedPeerIssuer().empty()) {
        EXPECT_EQ(options.expectedPeerIssuer(), server_connection->ssl()->issuerPeerCertificate());
      }
      if (!options.expectedPeerSubject().empty()) {
        EXPECT_EQ(options.expectedPeerSubject(),
                  server_connection->ssl()->subjectPeerCertificate());
      }
      if (!options.expectedLocalSubject().empty()) {
        EXPECT_EQ(options.expectedLocalSubject(),
                  server_connection->ssl()->subjectLocalCertificate());
      }
      if (!options.expectedPeerCert().empty()) {
        std::string urlencoded = absl::StrReplaceAll(
            options.expectedPeerCert(),
            {{"\r", ""}, {"\n", "%0A"}, {" ", "%20"}, {"+", "%2B"}, {"/", "%2F"}, {"=", "%3D"}});
        // Assert twice to ensure a cached value is returned and still valid.
        EXPECT_EQ(urlencoded, server_connection->ssl()->urlEncodedPemEncodedPeerCertificate());
        EXPECT_EQ(urlencoded, server_connection->ssl()->urlEncodedPemEncodedPeerCertificate());
      }
      if (!options.expectedPeerCertChain().empty()) {
        std::string cert_chain = absl::StrReplaceAll(
            options.expectedPeerCertChain(),
            {{"\r", ""}, {"\n", "%0A"}, {" ", "%20"}, {"+", "%2B"}, {"/", "%2F"}, {"=", "%3D"}});
        // Assert twice to ensure a cached value is returned and still valid.
        EXPECT_EQ(cert_chain, server_connection->ssl()->urlEncodedPemEncodedPeerCertificateChain());
        EXPECT_EQ(cert_chain, server_connection->ssl()->urlEncodedPemEncodedPeerCertificateChain());
      }
      if (!options.expectedValidFromTimePeerCert().empty()) {
        const std::string formatted = TestUtility::formatTime(
            server_connection->ssl()->validFromPeerCertificate().value(), "%b %e %H:%M:%S %Y GMT");
        EXPECT_EQ(options.expectedValidFromTimePeerCert(), formatted);
      }
      if (!options.expectedExpirationTimePeerCert().empty()) {
        const std::string formatted = TestUtility::formatTime(
            server_connection->ssl()->expirationPeerCertificate().value(), "%b %e %H:%M:%S %Y GMT");
        EXPECT_EQ(options.expectedExpirationTimePeerCert(), formatted);
      }
      if (options.expectNoCert()) {
        EXPECT_FALSE(server_connection->ssl()->peerCertificatePresented());
        EXPECT_FALSE(server_connection->ssl()->validFromPeerCertificate().has_value());
        EXPECT_FALSE(server_connection->ssl()->expirationPeerCertificate().has_value());
        EXPECT_EQ(EMPTY_STRING, server_connection->ssl()->sha256PeerCertificateDigest());
        EXPECT_EQ(EMPTY_STRING, server_connection->ssl()->sha1PeerCertificateDigest());
        EXPECT_EQ(EMPTY_STRING, server_connection->ssl()->urlEncodedPemEncodedPeerCertificate());
        EXPECT_EQ(EMPTY_STRING, server_connection->ssl()->subjectPeerCertificate());
        EXPECT_EQ(std::vector<std::string>{}, server_connection->ssl()->dnsSansPeerCertificate());
      }
      if (options.expectNoCertChain()) {
        EXPECT_EQ(EMPTY_STRING,
                  server_connection->ssl()->urlEncodedPemEncodedPeerCertificateChain());
      }

      const SslHandshakerImpl* ssl_socket =
          dynamic_cast<const SslHandshakerImpl*>(client_connection->ssl().get());
      SSL* client_ssl_socket = ssl_socket->ssl();
      const uint8_t* response_head;
      size_t response_len;
      SSL_get0_ocsp_response(client_ssl_socket, &response_head, &response_len);
      std::string ocsp_response{reinterpret_cast<const char*>(response_head), response_len};
      EXPECT_EQ(options.expectedOcspResponse(), ocsp_response);

      // By default, the session is not created with session resumption. The
      // client should see a session ID but the server should not.
      EXPECT_EQ(EMPTY_STRING, server_connection->ssl()->sessionId());
      EXPECT_NE(EMPTY_STRING, client_connection->ssl()->sessionId());

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
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { connect_second_time(); }));
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
    EXPECT_CALL(server_connection_callbacks, onEvent(Network::ConnectionEvent::LocalClose));
  } else {
    EXPECT_CALL(client_connection_callbacks, onEvent(Network::ConnectionEvent::RemoteClose))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { close_second_time(); }));
    EXPECT_CALL(server_connection_callbacks, onEvent(options.expectedServerCloseEvent()))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { close_second_time(); }));
  }

  if (options.expectedVerifyErrorCode() != -1) {
    EXPECT_LOG_CONTAINS("debug", X509_verify_cert_error_string(options.expectedVerifyErrorCode()),
                        dispatcher->run(Event::Dispatcher::RunType::Block));
  } else {
    dispatcher->run(Event::Dispatcher::RunType::Block);
  }

  if (!options.expectedServerStats().empty()) {
    EXPECT_EQ(1UL, server_stats_store.counter(options.expectedServerStats()).value());
  }

  if (!options.notExpectedClientStats().empty()) {
    EXPECT_EQ(0, client_stats_store.counter(options.notExpectedClientStats()).value());
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