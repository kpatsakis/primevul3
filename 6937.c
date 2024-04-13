TEST_P(SslReadBufferLimitTest, SmallReadsIntoSameSlice) {
  // write_size * num_writes must be large enough to cause buffer reserving fragmentation,
  // but smaller than one reservation so the expected slice to be 1.
  const uint32_t write_size = 1;
  const uint32_t num_writes = 12 * 1024;
  const uint32_t read_buffer_limit = write_size * num_writes;
  const uint32_t expected_chunk_size = write_size * num_writes;

  initialize();

  EXPECT_CALL(listener_callbacks_, onAccept_(_))
      .WillOnce(Invoke([&](Network::ConnectionSocketPtr& socket) -> void {
        server_connection_ = dispatcher_->createServerConnection(
            std::move(socket), server_ssl_socket_factory_->createTransportSocket(nullptr),
            stream_info_);
        server_connection_->setBufferLimits(read_buffer_limit);
        server_connection_->addConnectionCallbacks(server_callbacks_);
        server_connection_->addReadFilter(read_filter_);
        EXPECT_EQ("", server_connection_->nextProtocol());
        EXPECT_EQ(read_buffer_limit, server_connection_->bufferLimit());
      }));

  EXPECT_CALL(client_callbacks_, onEvent(Network::ConnectionEvent::Connected))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void { dispatcher_->exit(); }));
  dispatcher_->run(Event::Dispatcher::RunType::Block);

  uint32_t filter_seen = 0;

  EXPECT_CALL(*read_filter_, onNewConnection());
  EXPECT_CALL(*read_filter_, onData(_, _))
      .WillRepeatedly(Invoke([&](Buffer::Instance& data, bool) -> Network::FilterStatus {
        EXPECT_GE(expected_chunk_size, data.length());
        EXPECT_EQ(1, data.getRawSlices().size());
        filter_seen += data.length();
        data.drain(data.length());
        if (filter_seen == (write_size * num_writes)) {
          server_connection_->close(Network::ConnectionCloseType::FlushWrite);
        }
        return Network::FilterStatus::StopIteration;
      }));

  EXPECT_CALL(client_callbacks_, onEvent(Network::ConnectionEvent::RemoteClose))
      .WillOnce(Invoke([&](Network::ConnectionEvent) -> void {
        EXPECT_EQ((write_size * num_writes), filter_seen);
        dispatcher_->exit();
      }));

  for (uint32_t i = 0; i < num_writes; i++) {
    Buffer::OwnedImpl data(std::string(write_size, 'a'));
    client_connection_->write(data, false);
  }

  dispatcher_->run(Event::Dispatcher::RunType::Block);
}