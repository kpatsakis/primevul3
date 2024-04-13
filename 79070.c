int HttpStreamParser::ReadResponseBody(IOBuffer* buf, int buf_len,
                                       const CompletionCallback& callback) {
  DCHECK(io_state_ == STATE_BODY_PENDING || io_state_ == STATE_DONE);
  DCHECK(callback_.is_null());
  DCHECK(!callback.is_null());
  DCHECK_LE(buf_len, kMaxBufSize);

  if (io_state_ == STATE_DONE)
    return OK;

  user_read_buf_ = buf;
  user_read_buf_len_ = buf_len;
  io_state_ = STATE_READ_BODY;

  int result = DoLoop(OK);
  if (result == ERR_IO_PENDING)
    callback_ = callback;

  return result;
}
