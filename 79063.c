bool HttpStreamParser::IsConnectionReusable() const {
  return connection_->socket() && connection_->socket()->IsConnectedAndIdle();
}
