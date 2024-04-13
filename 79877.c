SavePackage::~SavePackage() {
  if (!finished_ && !canceled()) {
    Cancel(true);
  }

  CHECK(!download_);

  DCHECK(all_save_items_count_ == (waiting_item_queue_.size() +
                                   completed_count() +
                                   in_process_count()));
  while (!waiting_item_queue_.empty()) {
    SaveItem* save_item = waiting_item_queue_.front();
    waiting_item_queue_.pop();
    delete save_item;
  }

  STLDeleteValues(&saved_success_items_);
  STLDeleteValues(&in_progress_items_);
  STLDeleteValues(&saved_failed_items_);

  file_manager_ = NULL;
}
