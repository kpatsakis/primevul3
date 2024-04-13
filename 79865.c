void SavePackage::SaveCanceled(SaveItem* save_item) {
  file_manager_->RemoveSaveFile(save_item->save_id(),
                                save_item->url(),
                                this);
  if (save_item->save_id() != -1)
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&SaveFileManager::CancelSave,
                   file_manager_,
                   save_item->save_id()));
}
