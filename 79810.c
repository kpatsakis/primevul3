void ChromeDownloadManagerDelegate::Shutdown() {
  download_prefs_.reset();
#if !defined(OS_ANDROID)
  extension_event_router_.reset();
#endif
}
