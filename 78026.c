void WebContentsImpl::DocumentAvailableInMainFrame(
    RenderViewHost* render_view_host) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DocumentAvailableInMainFrame());
}
