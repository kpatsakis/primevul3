void WebContentsImpl::RenderViewDeleted(RenderViewHost* rvh) {
  ClearPowerSaveBlockers(rvh);
  render_manager_.RenderViewDeleted(rvh);
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, RenderViewDeleted(rvh));
}
