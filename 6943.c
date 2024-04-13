  TestUtilOptions& setExpectedOcspResponse(const std::string& expected_ocsp_response) {
    expected_ocsp_response_ = expected_ocsp_response;
    return *this;
  }