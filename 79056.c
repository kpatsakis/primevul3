int HttpStreamParser::DoSendHeaders(int result) {
  request_headers_->DidConsume(result);
  int bytes_remaining = request_headers_->BytesRemaining();
  if (bytes_remaining > 0) {
    if (bytes_remaining == request_headers_->size()) {
      response_->request_time = base::Time::Now();
    }
    result = connection_->socket()->Write(request_headers_,
                                          bytes_remaining,
                                          io_callback_);
  } else if (request_->upload_data_stream != NULL &&
             (request_->upload_data_stream->is_chunked() ||
              (request_->upload_data_stream->size() > 0 &&
               !request_->upload_data_stream->IsEOF()))) {
    net_log_.AddEvent(
        NetLog::TYPE_HTTP_TRANSACTION_SEND_REQUEST_BODY,
        base::Bind(&NetLogSendRequestBodyCallback,
                   request_->upload_data_stream->size(),
                   request_->upload_data_stream->is_chunked(),
                   false /* not merged */));
    io_state_ = STATE_SENDING_BODY;
    result = OK;
  } else {
    io_state_ = STATE_REQUEST_SENT;
  }
  return result;
}
