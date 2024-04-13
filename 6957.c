  TestUtilOptionsV2& setExpectedServerCertDigest(const std::string& expected_server_cert_digest) {
    expected_server_cert_digest_ = expected_server_cert_digest;
    return *this;
  }