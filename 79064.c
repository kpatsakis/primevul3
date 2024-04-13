bool HttpStreamParser::IsConnectionReused() const {
  ClientSocketHandle::SocketReuseType reuse_type = connection_->reuse_type();
  return connection_->is_reused() ||
         reuse_type == ClientSocketHandle::UNUSED_IDLE;
}
