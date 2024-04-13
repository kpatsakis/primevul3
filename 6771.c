  void initialize() {
    TestUtility::loadFromYaml(TestEnvironment::substitute(server_ctx_yaml_),
                              downstream_tls_context_);
    auto server_cfg =
        std::make_unique<ServerContextConfigImpl>(downstream_tls_context_, factory_context_);
    manager_ = std::make_unique<ContextManagerImpl>(time_system_);
    server_ssl_socket_factory_ = std::make_unique<ServerSslSocketFactory>(
        std::move(server_cfg), *manager_, server_stats_store_, std::vector<std::string>{});

    socket_ = std::make_shared<Network::Test::TcpListenSocketImmediateListen>(
        Network::Test::getCanonicalLoopbackAddress(GetParam()));
    listener_ = dispatcher_->createListener(socket_, listener_callbacks_, runtime_, true, false);

    TestUtility::loadFromYaml(TestEnvironment::substitute(client_ctx_yaml_), upstream_tls_context_);
    auto client_cfg =
        std::make_unique<ClientContextConfigImpl>(upstream_tls_context_, factory_context_);

    client_ssl_socket_factory_ = std::make_unique<ClientSslSocketFactory>(
        std::move(client_cfg), *manager_, client_stats_store_);
    auto transport_socket = client_ssl_socket_factory_->createTransportSocket(nullptr);
    client_transport_socket_ = transport_socket.get();
    client_connection_ =
        dispatcher_->createClientConnection(socket_->connectionInfoProvider().localAddress(),
                                            source_address_, std::move(transport_socket), nullptr);
    client_connection_->addConnectionCallbacks(client_callbacks_);
    client_connection_->connect();
    read_filter_ = std::make_shared<Network::MockReadFilter>();
  }