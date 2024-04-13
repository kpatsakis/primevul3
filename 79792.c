void ChromeDownloadManagerDelegate::DisableSafeBrowsing(DownloadItem* item) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
#if defined(FULL_SAFE_BROWSING)
  SafeBrowsingState* state = static_cast<SafeBrowsingState*>(
      item->GetUserData(&safe_browsing_id));
  if (!state) {
    state = new SafeBrowsingState();
    item->SetUserData(&safe_browsing_id, state);
  }
  state->SetVerdict(DownloadProtectionService::SAFE);
#endif
}
