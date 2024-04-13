ChromeDownloadManagerDelegate::ChromeDownloadManagerDelegate(Profile* profile)
    : profile_(profile),
      next_download_id_(0),
      download_prefs_(new DownloadPrefs(profile)) {
}
