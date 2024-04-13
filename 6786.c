  TestUtilOptions& setNotExpectedClientStats(const std::string& stat) {
    not_expected_client_stats_ = stat;
    return *this;
  }