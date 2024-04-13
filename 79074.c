void HttpStreamParser::SetConnectionReused() {
  connection_->set_is_reused(true);
}
