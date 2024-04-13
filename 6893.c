  TestUtilOptions& setExpectedExpirationTimePeerCert(const std::string& expected_expiration) {
    expected_expiration_peer_cert_ = expected_expiration;
    return *this;
  }