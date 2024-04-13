  virtual void OnJSONParseFailed(const std::string& error_message) {
    CHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
    manifest_parse_complete_ = true;
    error_ = error_message;
    parse_error_ = BeginInstallWithManifestFunction::MANIFEST_ERROR;
    ReportResultsIfComplete();
  }
