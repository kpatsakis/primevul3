bool HttpStreamParser::IsMoreDataBuffered() const {
  return read_buf_->offset() > read_buf_unused_offset_;
}
