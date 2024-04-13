void ProfileImplIOData::Handle::LazyInitialize() const {
  if (!initialized_) {
    io_data_->InitializeOnUIThread(profile_);
    PrefService* pref_service = profile_->GetPrefs();
    io_data_->http_server_properties_manager_.reset(
        new chrome_browser_net::HttpServerPropertiesManager(pref_service));
    ChromeNetworkDelegate::InitializeReferrersEnabled(
        io_data_->enable_referrers(), pref_service);
    io_data_->clear_local_state_on_exit()->Init(
        prefs::kClearSiteDataOnExit, pref_service, NULL);
    io_data_->clear_local_state_on_exit()->MoveToThread(BrowserThread::IO);
#if defined(ENABLE_SAFE_BROWSING)
    io_data_->safe_browsing_enabled()->Init(prefs::kSafeBrowsingEnabled,
        pref_service, NULL);
    io_data_->safe_browsing_enabled()->MoveToThread(BrowserThread::IO);
#endif
    initialized_ = true;
  }
}
