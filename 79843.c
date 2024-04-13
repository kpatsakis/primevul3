void SavePackage::DoSavingProcess() {
  if (save_type_ == SAVE_PAGE_TYPE_AS_COMPLETE_HTML) {
    SaveItem* save_item = NULL;
    if (waiting_item_queue_.size()) {
      DCHECK(wait_state_ == NET_FILES);
      save_item = waiting_item_queue_.front();
      if (save_item->save_source() != SaveFileCreateInfo::SAVE_FILE_FROM_DOM) {
        SaveNextFile(false);
      } else if (!in_process_count()) {
        wait_state_ = HTML_DATA;
        SaveNextFile(true);
      }
    } else if (in_process_count()) {
      DCHECK(wait_state_ == HTML_DATA);
    }
  } else {
    DCHECK(wait_state_ == NET_FILES);
    DCHECK((save_type_ == SAVE_PAGE_TYPE_AS_ONLY_HTML) ||
           (save_type_ == SAVE_PAGE_TYPE_AS_MHTML));
    if (waiting_item_queue_.size()) {
      DCHECK(all_save_items_count_ == waiting_item_queue_.size());
      SaveNextFile(false);
    }
  }
}
