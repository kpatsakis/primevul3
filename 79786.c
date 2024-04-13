void ChromeDownloadManagerDelegate::CheckClientDownloadDone(
    int32 download_id,
    DownloadProtectionService::DownloadCheckResult result) {
  DownloadItem* item = download_manager_->GetDownload(download_id);
  if (!item || (item->GetState() != DownloadItem::IN_PROGRESS))
    return;

  VLOG(2) << __FUNCTION__ << "() download = " << item->DebugString(false)
          << " verdict = " << result;
  if (item->GetDangerType() == content::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS ||
      item->GetDangerType() ==
      content::DOWNLOAD_DANGER_TYPE_MAYBE_DANGEROUS_CONTENT) {
    switch (result) {
      case DownloadProtectionService::SAFE:
        break;
      case DownloadProtectionService::DANGEROUS:
        item->OnContentCheckCompleted(
            content::DOWNLOAD_DANGER_TYPE_DANGEROUS_CONTENT);
        break;
      case DownloadProtectionService::UNCOMMON:
        item->OnContentCheckCompleted(
            content::DOWNLOAD_DANGER_TYPE_UNCOMMON_CONTENT);
        break;
    }
  }

  SafeBrowsingState* state = static_cast<SafeBrowsingState*>(
      item->GetUserData(&safe_browsing_id));
  state->SetVerdict(result);
}
