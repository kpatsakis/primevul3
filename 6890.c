  TestUtilOptions& enableOcspStapling() {
    ocsp_stapling_enabled_ = true;
    return *this;
  }