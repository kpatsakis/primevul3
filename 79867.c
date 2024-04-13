void SavePackage::SaveFinished(int32 save_id, int64 size, bool is_success) {
  SaveItem* save_item = LookupItemInProcessBySaveId(save_id);
  if (!save_item)
    return;

  save_item->Finish(size, is_success);
  file_manager_->RemoveSaveFile(save_id, save_item->url(), this);

  PutInProgressItemToSavedMap(save_item);

  if (download_ && download_->IsInProgress())
    download_->UpdateProgress(completed_count(), CurrentSpeed(), "");

  if (save_item->save_source() == SaveFileCreateInfo::SAVE_FILE_FROM_DOM &&
      save_item->url() == page_url_ && !save_item->received_bytes()) {
    Cancel(false);
    return;
  }

  if (canceled()) {
    DCHECK(finished_);
    return;
  }

  DoSavingProcess();

  CheckFinish();
}
