void WebContentsImpl::NotifyNavigationEntryCommitted(
    const LoadCommittedDetails& load_details) {
  FOR_EACH_OBSERVER(
      WebContentsObserver, observers_, NavigationEntryCommitted(load_details));
}
