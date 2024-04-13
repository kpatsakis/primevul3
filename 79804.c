bool ChromeDownloadManagerDelegate::ShouldCompleteDownload(
    DownloadItem* item,
    const base::Closure& user_complete_callback) {
  return IsDownloadReadyForCompletion(item, base::Bind(
      &ChromeDownloadManagerDelegate::ShouldCompleteDownloadInternal,
      this, item->GetId(), user_complete_callback));
}
