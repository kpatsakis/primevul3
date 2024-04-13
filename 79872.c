void SavePackage::Stop() {
  if (wait_state_ == INITIALIZE)
    return;

  DCHECK(canceled());
  if (in_process_count()) {
    SaveUrlItemMap::iterator it = in_progress_items_.begin();
    for (; it != in_progress_items_.end(); ++it) {
      SaveItem* save_item = it->second;
      DCHECK(save_item->state() == SaveItem::IN_PROGRESS);
      save_item->Cancel();
    }
    while (in_process_count())
      PutInProgressItemToSavedMap(in_progress_items_.begin()->second);
  }

  SaveIDList save_ids;
  for (SavedItemMap::iterator it = saved_success_items_.begin();
      it != saved_success_items_.end(); ++it)
    save_ids.push_back(it->first);
  for (SaveUrlItemMap::iterator it = saved_failed_items_.begin();
      it != saved_failed_items_.end(); ++it)
    save_ids.push_back(it->second->save_id());

  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(&SaveFileManager::RemoveSavedFileFromFileMap,
                 file_manager_,
                 save_ids));

  finished_ = true;
  wait_state_ = FAILED;

  if (download_) {
    download_->Cancel(false);
    FinalizeDownloadEntry();
  }
}
