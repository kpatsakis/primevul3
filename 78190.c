void WebContentsImpl::Stop() {
  render_manager_.Stop();
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, NavigationStopped());
}
