void NavigationController::NavigateToPendingEntry(ReloadType reload_type) {
  needs_reload_ = false;

  if (!pending_entry_) {
    DCHECK_NE(pending_entry_index_, -1);
    pending_entry_ = entries_[pending_entry_index_].get();
  }

  if (!tab_contents_->NavigateToPendingEntry(reload_type))
    DiscardNonCommittedEntries();
}
