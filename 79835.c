void SafeBrowsingBlockingPage::SetReportingPreference(bool report) {
  Profile* profile = Profile::FromBrowserContext(
      web_contents_->GetBrowserContext());
  PrefService* pref = profile->GetPrefs();
  pref->SetBoolean(prefs::kSafeBrowsingReportingEnabled, report);
}
