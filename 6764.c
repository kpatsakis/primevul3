  TestUtilOptions& setExpectedTransportFailureReasonContains(
      const std::string& expected_transport_failure_reason_contains) {
    expected_transport_failure_reason_contains_ = expected_transport_failure_reason_contains;
    return *this;
  }