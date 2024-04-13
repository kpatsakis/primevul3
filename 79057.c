int HttpStreamParser::DoSendRequestReadingBody(int result) {
  DCHECK_GE(result, 0);  // There won't be errors.

  if (request_->upload_data_stream->is_chunked()) {
    if (result == 0) {  // Reached the end.
      DCHECK(request_->upload_data_stream->IsEOF());
      sent_last_chunk_ = true;
    }
    const base::StringPiece payload(request_body_read_buf_->data(), result);
    request_body_send_buf_->Clear();
    result = EncodeChunk(payload,
                         request_body_send_buf_->data(),
                         request_body_send_buf_->capacity());
  }

  if (result == 0) {  // Reached the end.
    DCHECK(request_->upload_data_stream->IsEOF());
    DCHECK(!request_->upload_data_stream->is_chunked());
    io_state_ = STATE_REQUEST_SENT;
  } else if (result > 0) {
    request_body_send_buf_->DidAppend(result);
    result = 0;
    io_state_ = STATE_SENDING_BODY;
  }
  return result;
}
