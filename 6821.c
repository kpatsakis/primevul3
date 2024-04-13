  TestUtilOptions& setExpectedLocalUri(const std::string& expected_local_uri) {
    expected_local_uri_ = {expected_local_uri};
    return *this;
  }