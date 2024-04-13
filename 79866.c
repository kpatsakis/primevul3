void SavePackage::SaveFailed(const GURL& save_url) {
  SaveUrlItemMap::iterator it = in_progress_items_.find(save_url.spec());
  if (it == in_progress_items_.end()) {
    NOTREACHED();  // Should not exist!
    return;
  }
  SaveItem* save_item = it->second;

  save_item->Finish(0, false);

  PutInProgressItemToSavedMap(save_item);

  if (download_ && download_->IsInProgress())
    download_->UpdateProgress(completed_count(), CurrentSpeed(), "");

  if ((save_type_ == SAVE_PAGE_TYPE_AS_ONLY_HTML) ||
      (save_type_ == SAVE_PAGE_TYPE_AS_MHTML) ||
      (save_item->save_source() == SaveFileCreateInfo::SAVE_FILE_FROM_DOM)) {
    Cancel(true);
  }

  if (canceled()) {
    DCHECK(finished_);
    return;
  }

  DoSavingProcess();

  CheckFinish();
}
