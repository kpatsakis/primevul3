void ChromeDownloadManagerDelegate::SubstituteDriveDownloadPathCallback(
    int32 download_id,
    const content::DownloadTargetCallback& callback,
    bool should_prompt,
    bool is_forced_path,
    content::DownloadDangerType danger_type,
    const FilePath& suggested_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DownloadItem* download =
      download_manager_->GetDownload(download_id);
  if (!download || (download->GetState() != DownloadItem::IN_PROGRESS))
    return;

  if (suggested_path.empty()) {
    callback.Run(FilePath(),
                 DownloadItem::TARGET_DISPOSITION_OVERWRITE,
                 danger_type,
                 FilePath());
    return;
  }

  GetReservedPath(
      *download, suggested_path, download_prefs_->DownloadPath(),
      !is_forced_path,
      base::Bind(&ChromeDownloadManagerDelegate::OnPathReservationAvailable,
                 this, download->GetId(), callback, should_prompt,
                 danger_type));
}
