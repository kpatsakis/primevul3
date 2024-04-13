int HttpStreamParser::DoReadBody() {
  io_state_ = STATE_READ_BODY_COMPLETE;

  if (read_buf_->offset()) {
    int available = read_buf_->offset() - read_buf_unused_offset_;
    if (available) {
      CHECK_GT(available, 0);
      int bytes_from_buffer = std::min(available, user_read_buf_len_);
      memcpy(user_read_buf_->data(),
             read_buf_->StartOfBuffer() + read_buf_unused_offset_,
             bytes_from_buffer);
      read_buf_unused_offset_ += bytes_from_buffer;
      if (bytes_from_buffer == available) {
        read_buf_->SetCapacity(0);
        read_buf_unused_offset_ = 0;
      }
      return bytes_from_buffer;
    } else {
      read_buf_->SetCapacity(0);
      read_buf_unused_offset_ = 0;
    }
  }

  if (IsResponseBodyComplete())
    return 0;

  DCHECK_EQ(0, read_buf_->offset());
  return connection_->socket()->Read(user_read_buf_, user_read_buf_len_,
                                     io_callback_);
}
