void ChromeDownloadManagerDelegate::SetDownloadManager(DownloadManager* dm) {
  download_manager_ = dm;
#if !defined(OS_ANDROID)
  extension_event_router_.reset(new ExtensionDownloadsEventRouter(
      profile_, download_manager_));
#endif
}
