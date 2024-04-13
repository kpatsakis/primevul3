  const std::string& GetBuffer() {
    if (!has_accept_header_) {
      if (!buffer_.empty())
        buffer_.append("\r\n");
      buffer_.append("Accept: */*");
      has_accept_header_ = true;
    }
    return buffer_;
  }
