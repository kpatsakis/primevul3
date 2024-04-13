int HttpStreamParser::ReadResponseHeaders(const CompletionCallback& callback) {
  DCHECK(io_state_ == STATE_REQUEST_SENT || io_state_ == STATE_DONE);
  DCHECK(callback_.is_null());
  DCHECK(!callback.is_null());

  if (io_state_ == STATE_DONE)
    return ERR_CONNECTION_CLOSED;

  int result = OK;
  io_state_ = STATE_READ_HEADERS;

  if (read_buf_->offset() > 0) {
    result = read_buf_->offset() - read_buf_unused_offset_;
    read_buf_->set_offset(read_buf_unused_offset_);
  }
  if (result > 0)
    io_state_ = STATE_READ_HEADERS_COMPLETE;

  result = DoLoop(result);
  if (result == ERR_IO_PENDING)
    callback_ = callback;

  return result > 0 ? OK : result;
}
