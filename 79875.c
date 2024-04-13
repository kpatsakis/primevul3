bool SavePackage::UpdateSaveProgress(int32 save_id,
                                     int64 size,
                                     bool write_success) {
  SaveItem* save_item = LookupItemInProcessBySaveId(save_id);
  if (!save_item)
    return false;

  save_item->Update(size);

  if (!write_success) {
    Cancel(false);
  }
  return true;
}
