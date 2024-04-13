void ChromeDownloadManagerDelegate::CheckVisitedReferrerBeforeDone(
    int32 download_id,
    const content::DownloadTargetCallback& callback,
    content::DownloadDangerType danger_type,
    bool visited_referrer_before) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  DownloadItem* download =
      download_manager_->GetDownload(download_id);
  if (!download || (download->GetState() != DownloadItem::IN_PROGRESS))
    return;

  bool should_prompt = (download->GetTargetDisposition() ==
                        DownloadItem::TARGET_DISPOSITION_PROMPT);
  bool is_forced_path = !download->GetForcedFilePath().empty();
  FilePath suggested_path;

  if (!is_forced_path) {
    FilePath generated_name;
    GenerateFileNameFromRequest(
        *download,
        &generated_name,
        profile_->GetPrefs()->GetString(prefs::kDefaultCharset));

    if (download_prefs_->PromptForDownload()) {
      if (!download_crx_util::IsExtensionDownload(*download) &&
          !ShouldOpenFileBasedOnExtension(generated_name))
        should_prompt = true;
    }
    if (download_prefs_->IsDownloadPathManaged())
      should_prompt = false;

    FilePath target_directory;
    if (should_prompt && !last_download_path_.empty())
      target_directory = last_download_path_;
    else
      target_directory = download_prefs_->DownloadPath();
    suggested_path = target_directory.Append(generated_name);
  } else {
    DCHECK(!should_prompt);
    suggested_path = download->GetForcedFilePath();
  }

  if (ShouldOpenWithWebIntents(download)) {
    download->SetDisplayName(suggested_path.BaseName());
    suggested_path = suggested_path.AddExtension(kWebIntentsFileExtension);
  }

  if (danger_type == content::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS) {
    if (!should_prompt && !is_forced_path &&
        IsDangerousFile(*download, suggested_path, visited_referrer_before)) {
      danger_type = content::DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE;
    }

#if defined(FULL_SAFE_BROWSING)
    DownloadProtectionService* service = GetDownloadProtectionService();
    if (service && service->enabled()) {
      DownloadProtectionService::DownloadInfo info =
          DownloadProtectionService::DownloadInfo::FromDownloadItem(*download);
      info.target_file = suggested_path;
      if (service->IsSupportedDownload(info))
        danger_type = content::DOWNLOAD_DANGER_TYPE_MAYBE_DANGEROUS_CONTENT;
    }
#endif
  } else {
    DCHECK_EQ(content::DOWNLOAD_DANGER_TYPE_DANGEROUS_URL, danger_type);
  }

#if defined (OS_CHROMEOS)
  drive::DriveDownloadObserver::SubstituteDriveDownloadPath(
      profile_, suggested_path, download,
      base::Bind(
          &ChromeDownloadManagerDelegate::SubstituteDriveDownloadPathCallback,
          this, download->GetId(), callback, should_prompt, is_forced_path,
          danger_type));
#else
  GetReservedPath(
      *download, suggested_path, download_prefs_->DownloadPath(),
      !is_forced_path,
      base::Bind(&ChromeDownloadManagerDelegate::OnPathReservationAvailable,
                 this, download->GetId(), callback, should_prompt,
                 danger_type));
#endif
}
