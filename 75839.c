  void StartWorkOnIOThread() {
    CHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
    utility_host_ = new UtilityProcessHost(this, BrowserThread::IO);
    utility_host_->StartBatchMode();
    if (icon_data_.empty())
      icon_decode_complete_ = true;
    else
      utility_host_->StartImageDecodingBase64(icon_data_);
    utility_host_->StartJSONParsing(manifest_);
  }
