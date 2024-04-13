  TestUtilOptions& setExpectedPeerCert(const std::string& expected_peer_cert) {
    expected_peer_cert_ = expected_peer_cert;
    return *this;
  }