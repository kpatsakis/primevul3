  scoped_refptr<TransportSocketParams> GetTcpParams() {
    if (GetParam() != HTTP)
      return scoped_refptr<TransportSocketParams>();
    return ignored_transport_socket_params_;
  }
