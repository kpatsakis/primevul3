  TestUtilOptions& setExpectedPeerCertChain(const std::string& expected_peer_cert_chain) {
    expected_peer_cert_chain_ = expected_peer_cert_chain;
    return *this;
  }