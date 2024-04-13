bool WebContentsImpl::IsActiveEntry(int32 page_id) {
  NavigationEntryImpl* active_entry =
      NavigationEntryImpl::FromNavigationEntry(controller_.GetActiveEntry());
  return (active_entry != NULL &&
          active_entry->site_instance() == GetSiteInstance() &&
          active_entry->GetPageID() == page_id);
}
