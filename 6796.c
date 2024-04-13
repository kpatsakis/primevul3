  TestUtilOptionsV2& setTransportSocketOptions(
      Network::TransportSocketOptionsConstSharedPtr transport_socket_options) {
    transport_socket_options_ = transport_socket_options;
    return *this;
  }