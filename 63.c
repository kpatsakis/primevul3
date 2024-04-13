Network::Network(const Network& network) {
  service_path_ = network.service_path();
  device_path_ = network.device_path();
  ip_address_ = network.ip_address();
  type_ = network.type();
  state_ = network.state();
  error_ = network.error();
}
