int HttpStreamParser::DoSendBody(int result) {

  request_body_send_buf_->DidConsume(result);
  if (request_body_send_buf_->BytesRemaining() > 0) {
    return connection_->socket()->Write(
        request_body_send_buf_,
        request_body_send_buf_->BytesRemaining(),
        io_callback_);
  }

  if (request_->upload_data_stream->is_chunked() && sent_last_chunk_) {
    io_state_ = STATE_REQUEST_SENT;
    return OK;
  }

  request_body_read_buf_->Clear();
  io_state_ = STATE_SEND_REQUEST_READING_BODY;
  return request_->upload_data_stream->Read(request_body_read_buf_,
                                            request_body_read_buf_->capacity(),
                                            io_callback_);
}
