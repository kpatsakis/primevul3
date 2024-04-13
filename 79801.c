void ChromeDownloadManagerDelegate::OpenDownload(DownloadItem* download) {
  platform_util::OpenItem(download->GetFullPath());
}
