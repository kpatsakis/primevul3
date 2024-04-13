int HttpStreamParser::ParseResponseHeaders() {
  int end_offset = -1;

  if (response_header_start_offset_ < 0) {
    response_header_start_offset_ = HttpUtil::LocateStartOfStatusLine(
        read_buf_->StartOfBuffer() + read_buf_unused_offset_,
        read_buf_->offset() - read_buf_unused_offset_);
  }

  if (response_header_start_offset_ >= 0) {
    end_offset = HttpUtil::LocateEndOfHeaders(
        read_buf_->StartOfBuffer() + read_buf_unused_offset_,
        read_buf_->offset() - read_buf_unused_offset_,
        response_header_start_offset_);
  } else if (read_buf_->offset() - read_buf_unused_offset_ >= 8) {
    end_offset = 0;
  }

  if (end_offset == -1)
    return -1;

  int rv = DoParseResponseHeaders(end_offset);
  if (rv < 0)
    return rv;
  return end_offset + read_buf_unused_offset_;
}
