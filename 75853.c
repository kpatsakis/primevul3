void NavigationController::GoBack() {
  if (!CanGoBack()) {
    NOTREACHED();
    return;
  }

  if (tab_contents_->interstitial_page()) {
    tab_contents_->interstitial_page()->DontProceed();
    return;
  }

  int current_index = GetCurrentEntryIndex();

  DiscardNonCommittedEntries();

  pending_entry_index_ = current_index - 1;
  entries_[pending_entry_index_]->set_transition_type(
      entries_[pending_entry_index_]->transition_type() |
      PageTransition::FORWARD_BACK);
  NavigateToPendingEntry(NO_RELOAD);
}
