void ChromotingInstance::NotifyClientDimensions(int width, int height) {
  if (!IsConnected()) {
    return;
  }
  protocol::ClientDimensions client_dimensions;
  client_dimensions.set_width(width);
  client_dimensions.set_height(height);
  host_connection_->host_stub()->NotifyClientDimensions(client_dimensions);
}
