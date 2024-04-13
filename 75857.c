void NavigationController::LoadIfNecessary() {
  if (!needs_reload_)
    return;

  pending_entry_index_ = last_committed_entry_index_;
  NavigateToPendingEntry(NO_RELOAD);
}
