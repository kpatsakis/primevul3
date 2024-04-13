void HttpStreamParser::Close(bool not_reusable) {
  if (not_reusable && connection_->socket())
    connection_->socket()->Disconnect();
  connection_->Reset();
}
