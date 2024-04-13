HttpStreamParser::HttpStreamParser(ClientSocketHandle* connection,
                                   const HttpRequestInfo* request,
                                   GrowableIOBuffer* read_buffer,
                                   const BoundNetLog& net_log)
    : io_state_(STATE_NONE),
      request_(request),
      request_headers_(NULL),
      read_buf_(read_buffer),
      read_buf_unused_offset_(0),
      response_header_start_offset_(-1),
      response_body_length_(-1),
      response_body_read_(0),
      chunked_decoder_(NULL),
      user_read_buf_(NULL),
      user_read_buf_len_(0),
      connection_(connection),
      net_log_(net_log),
      sent_last_chunk_(false),
      weak_ptr_factory_(this) {
  io_callback_ = base::Bind(&HttpStreamParser::OnIOComplete,
                            weak_ptr_factory_.GetWeakPtr());
}
