void WebContentsImpl::RenderViewForInterstitialPageCreated(
    RenderViewHost* render_view_host) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    RenderViewForInterstitialPageCreated(render_view_host));
}
