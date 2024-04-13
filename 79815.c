void SafeBrowsingBlockingPage::FinishMalwareDetails(int64 delay_ms) {
  if (malware_details_ == NULL)
    return;  // Not all interstitials have malware details (eg phishing).

  if (IsPrefEnabled(prefs::kSafeBrowsingReportingEnabled)) {
    BrowserThread::PostDelayedTask(
        BrowserThread::IO, FROM_HERE,
        base::Bind(&MalwareDetails::FinishCollection, malware_details_.get()),
        base::TimeDelta::FromMilliseconds(delay_ms));
  }
}
