int HttpStreamParser::DoReadHeaders() {
  io_state_ = STATE_READ_HEADERS_COMPLETE;

  if (read_buf_->RemainingCapacity() == 0)
    read_buf_->SetCapacity(read_buf_->capacity() + kHeaderBufInitialSize);

  CHECK(read_buf_->data());

  return connection_->socket()->Read(read_buf_,
                                     read_buf_->RemainingCapacity(),
                                     io_callback_);
}
