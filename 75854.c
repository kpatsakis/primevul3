void NavigationController::GoForward() {
  if (!CanGoForward()) {
    NOTREACHED();
    return;
  }

  if (tab_contents_->interstitial_page()) {
    tab_contents_->interstitial_page()->CancelForNavigation();
  }

  bool transient = (transient_entry_index_ != -1);

  int current_index = GetCurrentEntryIndex();

  DiscardNonCommittedEntries();

  pending_entry_index_ = current_index;
  if (!transient)
    pending_entry_index_++;

  entries_[pending_entry_index_]->set_transition_type(
      entries_[pending_entry_index_]->transition_type() |
      PageTransition::FORWARD_BACK);
  NavigateToPendingEntry(NO_RELOAD);
}
