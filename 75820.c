  virtual void OnDecodeImageSucceeded(const SkBitmap& decoded_image) {
    CHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
    icon_ = decoded_image;
    icon_decode_complete_ = true;
    ReportResultsIfComplete();
  }
