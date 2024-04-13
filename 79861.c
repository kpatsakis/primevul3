void SavePackage::OnMHTMLGenerated(const FilePath& path, int64 size) {
  if (size <= 0) {
    Cancel(false);
    return;
  }
  wrote_to_completed_file_ = true;

  if (download_->IsInProgress()) {
    download_->SetTotalBytes(size);
    download_->UpdateProgress(size, 0, "");
    download_->OnAllDataSaved(DownloadItem::kEmptyFileHash);
  }

  if (!download_manager_->GetDelegate()) {
    Finish();
    return;
  }

  if (download_manager_->GetDelegate()->ShouldCompleteDownload(
          download_, base::Bind(&SavePackage::Finish, this))) {
    Finish();
  }
}
