void ChromeDownloadManagerDelegate::ShowDownloadInShell(
    DownloadItem* download) {
  platform_util::ShowItemInFolder(download->GetFullPath());
}
