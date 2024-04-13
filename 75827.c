  void ReportResultsIfComplete() {
    CHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

    if (!icon_decode_complete_ || !manifest_parse_complete_)
      return;

    utility_host_->EndBatchMode();
    utility_host_ = NULL;

    BrowserThread::PostTask(
        BrowserThread::UI,
        FROM_HERE,
        NewRunnableMethod(this,
                          &SafeBeginInstallHelper::ReportResultFromUIThread));
  }
