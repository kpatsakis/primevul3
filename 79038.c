  void Initialize(MockRead* reads, size_t reads_count,
                  MockWrite* writes, size_t writes_count,
                  MockRead* spdy_reads, size_t spdy_reads_count,
                  MockWrite* spdy_writes, size_t spdy_writes_count) {
    if (GetParam() == SPDY) {
      data_.reset(new DeterministicSocketData(spdy_reads, spdy_reads_count,
                                              spdy_writes, spdy_writes_count));
    } else {
      data_.reset(new DeterministicSocketData(reads, reads_count, writes,
                                              writes_count));
    }

    data_->set_connect_data(MockConnect(SYNCHRONOUS, OK));
    data_->StopAfter(2);  // Request / Response

    socket_factory().AddSocketDataProvider(data_.get());

    if (GetParam() != HTTP) {
      ssl_data_.reset(new SSLSocketDataProvider(SYNCHRONOUS, OK));
      if (GetParam() == SPDY) {
        InitializeSpdySsl();
      }
      socket_factory().AddSSLSocketDataProvider(ssl_data_.get());
    }
  }
