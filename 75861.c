void NavigationController::PruneAllButActive() {
  if (transient_entry_index_ != -1) {
    DCHECK_EQ(entry_count() - 1, transient_entry_index_);
    entries_.erase(entries_.begin(), entries_.begin() + transient_entry_index_);
    transient_entry_index_ = 0;
    last_committed_entry_index_ = -1;
    pending_entry_index_ = -1;
  } else if (!pending_entry_) {
    if (!entry_count())
      return;

    DCHECK(last_committed_entry_index_ >= 0);
    entries_.erase(entries_.begin(),
                   entries_.begin() + last_committed_entry_index_);
    entries_.erase(entries_.begin() + 1, entries_.end());
    last_committed_entry_index_ = 0;
  } else if (pending_entry_index_ != -1) {
    entries_.erase(entries_.begin(), entries_.begin() + pending_entry_index_);
    entries_.erase(entries_.begin() + 1, entries_.end());
    pending_entry_index_ = 0;
    last_committed_entry_index_ = 0;
  } else {
    pending_entry_index_ = -1;
    last_committed_entry_index_ = -1;
    entries_.clear();
  }

  if (tab_contents_->interstitial_page()) {
    tab_contents_->interstitial_page()->set_reload_on_dont_proceed(true);
  }
}
