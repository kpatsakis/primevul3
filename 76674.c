  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) {
    process_host_->Send(new PpapiMsg_SetNetworkState(
        type != net::NetworkChangeNotifier::CONNECTION_NONE));
  }
