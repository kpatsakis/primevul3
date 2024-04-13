  virtual void OnDecodeImageFailed() {
    CHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
    icon_decode_complete_ = true;
    error_ = std::string(kImageDecodeError);
    parse_error_ = BeginInstallWithManifestFunction::ICON_ERROR;
    ReportResultsIfComplete();
  }
