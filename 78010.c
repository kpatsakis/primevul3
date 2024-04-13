void WebContentsImpl::DidCancelLoading() {
  controller_.DiscardNonCommittedEntries();

  NotifyNavigationStateChanged(INVALIDATE_TYPE_URL);
}
