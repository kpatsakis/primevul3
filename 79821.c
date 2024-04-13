void SafeBrowsingBlockingPage::OnDontProceed() {
  RecordUserReactionTime(kNavigatedAwayMetaCommand);
  if (proceeded_)
    return;

  RecordUserAction(DONT_PROCEED);
  FinishMalwareDetails(0);  // No delay

  NotifySafeBrowsingUIManager(ui_manager_, unsafe_resources_, false);

  UnsafeResourceMap* unsafe_resource_map = GetUnsafeResourcesMap();
  UnsafeResourceMap::iterator iter = unsafe_resource_map->find(web_contents_);
  if (iter != unsafe_resource_map->end() && !iter->second.empty()) {
    NotifySafeBrowsingUIManager(ui_manager_, iter->second, false);
    unsafe_resource_map->erase(iter);
  }

  int last_committed_index =
      web_contents_->GetController().GetLastCommittedEntryIndex();
  if (navigation_entry_index_to_remove_ != -1 &&
      navigation_entry_index_to_remove_ != last_committed_index &&
      !web_contents_->IsBeingDestroyed()) {
    web_contents_->GetController().RemoveEntryAtIndex(
        navigation_entry_index_to_remove_);
    navigation_entry_index_to_remove_ = -1;
  }
}
