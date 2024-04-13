void SafeBrowsingBlockingPage::OverrideRendererPrefs(
      content::RendererPreferences* prefs) {
  Profile* profile = Profile::FromBrowserContext(
      web_contents_->GetBrowserContext());
  renderer_preferences_util::UpdateFromSystemSettings(prefs, profile);
 }
