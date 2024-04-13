void SavePackage::Finish() {
  if (canceled())
    return;

  wait_state_ = SUCCESSFUL;
  finished_ = true;

  RecordSavePackageEvent(SAVE_PACKAGE_FINISHED);

  if (wrote_to_completed_file_) {
    RecordSavePackageEvent(SAVE_PACKAGE_WRITE_TO_COMPLETED);
  }

  if (wrote_to_failed_file_) {
    RecordSavePackageEvent(SAVE_PACKAGE_WRITE_TO_FAILED);
  }

  SaveIDList save_ids;
  for (SaveUrlItemMap::iterator it = saved_failed_items_.begin();
       it != saved_failed_items_.end(); ++it)
    save_ids.push_back(it->second->save_id());

  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(&SaveFileManager::RemoveSavedFileFromFileMap,
                 file_manager_,
                 save_ids));

  if (download_) {
    if (download_->IsInProgress()) {
      if (save_type_ != SAVE_PAGE_TYPE_AS_MHTML) {
        download_->UpdateProgress(all_save_items_count_, CurrentSpeed(), "");
        download_->OnAllDataSaved(DownloadItem::kEmptyFileHash);
      }
      download_->MarkAsComplete();
    }
    FinalizeDownloadEntry();
  }
}
