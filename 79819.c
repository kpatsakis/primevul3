bool SafeBrowsingBlockingPage::IsPrefEnabled(const char* pref) {
  Profile* profile =
      Profile::FromBrowserContext(web_contents_->GetBrowserContext());
  return profile->GetPrefs()->GetBoolean(pref);
}
