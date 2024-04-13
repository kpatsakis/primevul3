SaveItem* SavePackage::LookupItemInProcessBySaveId(int32 save_id) {
  if (in_process_count()) {
    for (SaveUrlItemMap::iterator it = in_progress_items_.begin();
        it != in_progress_items_.end(); ++it) {
      SaveItem* save_item = it->second;
      DCHECK(save_item->state() == SaveItem::IN_PROGRESS);
      if (save_item->save_id() == save_id)
        return save_item;
    }
  }
  return NULL;
}
