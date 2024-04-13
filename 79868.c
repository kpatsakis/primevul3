void SavePackage::SaveNextFile(bool process_all_remaining_items) {
  DCHECK(web_contents());
  DCHECK(waiting_item_queue_.size());

  do {
    SaveItem* save_item = waiting_item_queue_.front();
    waiting_item_queue_.pop();

    SaveUrlItemMap::iterator it = in_progress_items_.find(
        save_item->url().spec());
    DCHECK(it == in_progress_items_.end());
    in_progress_items_[save_item->url().spec()] = save_item;
    save_item->Start();
    file_manager_->SaveURL(save_item->url(),
                           save_item->referrer(),
                           web_contents()->GetRenderProcessHost()->GetID(),
                           routing_id(),
                           save_item->save_source(),
                           save_item->full_path(),
                           web_contents()->
                               GetBrowserContext()->GetResourceContext(),
                           this);
  } while (process_all_remaining_items && waiting_item_queue_.size());
}
