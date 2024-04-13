  SslSocketTest()
      : dispatcher_(api_->allocateDispatcher("test_thread")),
        stream_info_(api_->timeSource(), nullptr) {}