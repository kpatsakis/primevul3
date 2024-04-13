  TestUtilOptionsV2& setExpectedALPNProtocol(const std::string& expected_alpn_protocol) {
    expected_alpn_protocol_ = expected_alpn_protocol;
    return *this;
  }