int HttpStreamParser::SendRequest(const std::string& request_line,
                                  const HttpRequestHeaders& headers,
                                  HttpResponseInfo* response,
                                  const CompletionCallback& callback) {
  DCHECK_EQ(STATE_NONE, io_state_);
  DCHECK(callback_.is_null());
  DCHECK(!callback.is_null());
  DCHECK(response);

  net_log_.AddEvent(
      NetLog::TYPE_HTTP_TRANSACTION_SEND_REQUEST_HEADERS,
      base::Bind(&HttpRequestHeaders::NetLogCallback,
                 base::Unretained(&headers),
                 &request_line));

  DVLOG(1) << __FUNCTION__ << "()"
           << " request_line = \"" << request_line << "\""
           << " headers = \"" << headers.ToString() << "\"";
  response_ = response;

  IPEndPoint ip_endpoint;
  int result = connection_->socket()->GetPeerAddress(&ip_endpoint);
  if (result != OK)
    return result;
  response_->socket_address = HostPortPair::FromIPEndPoint(ip_endpoint);

  std::string request = request_line + headers.ToString();

  if (request_->upload_data_stream != NULL) {
    request_body_send_buf_ = new SeekableIOBuffer(kRequestBodyBufferSize);
    if (request_->upload_data_stream->is_chunked()) {
      request_body_read_buf_ =
          new SeekableIOBuffer(kRequestBodyBufferSize - kChunkHeaderFooterSize);
    } else {
      request_body_read_buf_ = request_body_send_buf_;
    }
  }

  io_state_ = STATE_SENDING_HEADERS;

  bool did_merge = false;
  if (ShouldMergeRequestHeadersAndBody(request, request_->upload_data_stream)) {
    size_t merged_size = request.size() + request_->upload_data_stream->size();
    scoped_refptr<IOBuffer> merged_request_headers_and_body(
        new IOBuffer(merged_size));
    request_headers_ = new DrainableIOBuffer(
        merged_request_headers_and_body, merged_size);

    memcpy(request_headers_->data(), request.data(), request.size());
    request_headers_->DidConsume(request.size());

    size_t todo = request_->upload_data_stream->size();
    while (todo) {
      int consumed = request_->upload_data_stream->Read(request_headers_, todo,
                                                        CompletionCallback());
      DCHECK_GT(consumed, 0);  // Read() won't fail if not chunked.
      request_headers_->DidConsume(consumed);
      todo -= consumed;
    }
    DCHECK(request_->upload_data_stream->IsEOF());
    request_headers_->SetOffset(0);
    did_merge = true;

    net_log_.AddEvent(
        NetLog::TYPE_HTTP_TRANSACTION_SEND_REQUEST_BODY,
        base::Bind(&NetLogSendRequestBodyCallback,
                   request_->upload_data_stream->size(),
                   false, /* not chunked */
                   true /* merged */));
  }

  if (!did_merge) {
    scoped_refptr<StringIOBuffer> headers_io_buf(new StringIOBuffer(request));
    request_headers_ = new DrainableIOBuffer(headers_io_buf,
                                             headers_io_buf->size());
  }

  result = DoLoop(OK);
  if (result == ERR_IO_PENDING)
    callback_ = callback;

  return result > 0 ? OK : result;
}
