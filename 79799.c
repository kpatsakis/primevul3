void ChromeDownloadManagerDelegate::OnPathReservationAvailable(
    int32 download_id,
    const content::DownloadTargetCallback& callback,
    bool should_prompt,
    content::DownloadDangerType danger_type,
    const FilePath& reserved_path,
    bool reserved_path_verified) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DownloadItem* download =
      download_manager_->GetDownload(download_id);
  if (!download || (download->GetState() != DownloadItem::IN_PROGRESS))
    return;
  if (should_prompt || !reserved_path_verified) {
    ChooseDownloadPath(
        download, reserved_path,
        base::Bind(&ChromeDownloadManagerDelegate::OnTargetPathDetermined,
                   this, download_id, callback,
                   DownloadItem::TARGET_DISPOSITION_PROMPT, danger_type));
  } else {
    OnTargetPathDetermined(download_id, callback,
                           DownloadItem::TARGET_DISPOSITION_OVERWRITE,
                           danger_type, reserved_path);
  }
}
