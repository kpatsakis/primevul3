  TestUtilOptions& setExpectedServerCloseEvent(Network::ConnectionEvent expected_event) {
    expected_server_close_event_ = expected_event;
    return *this;
  }