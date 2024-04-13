  void disconnect() {
    EXPECT_CALL(client_callbacks_, onEvent(Network::ConnectionEvent::LocalClose));
    EXPECT_CALL(server_callbacks_, onEvent(Network::ConnectionEvent::RemoteClose))
        .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { dispatcher_->exit(); }));

    client_connection_->close(Network::ConnectionCloseType::NoFlush);
    dispatcher_->run(Event::Dispatcher::RunType::Block);
  }