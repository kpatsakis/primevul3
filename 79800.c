void ChromeDownloadManagerDelegate::OnTargetPathDetermined(
    int32 download_id,
    const content::DownloadTargetCallback& callback,
    DownloadItem::TargetDisposition disposition,
    content::DownloadDangerType danger_type,
    const FilePath& target_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  FilePath intermediate_path;
  DownloadItem* download =
      download_manager_->GetDownload(download_id);
  if (!download || (download->GetState() != DownloadItem::IN_PROGRESS))
    return;

  if (!target_path.empty()) {
    intermediate_path = GetIntermediatePath(target_path, danger_type);

    if (disposition == DownloadItem::TARGET_DISPOSITION_PROMPT &&
        !download->IsTemporary())
      last_download_path_ = target_path.DirName();
  }
  callback.Run(target_path, disposition, danger_type, intermediate_path);
}
