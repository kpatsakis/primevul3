void WebContentsImpl::OnGoToEntryAtOffset(int offset) {
  if (!delegate_ || delegate_->OnGoToEntryOffset(offset)) {
    NavigationEntryImpl* entry = NavigationEntryImpl::FromNavigationEntry(
        controller_.GetEntryAtOffset(offset));
    if (!entry)
      return;
    entry->SetTransitionType(
        PageTransitionFromInt(
            entry->GetTransitionType() |
            PAGE_TRANSITION_FORWARD_BACK));
    NavigateToEntry(*entry, NavigationControllerImpl::NO_RELOAD);

    if (!entry->site_instance() &&
        entry->restore_type() != NavigationEntryImpl::RESTORE_NONE) {
      entry->set_site_instance(
          static_cast<SiteInstanceImpl*>(GetPendingSiteInstance()));
    }
  }
}
