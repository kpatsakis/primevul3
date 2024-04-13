void WebContentsImpl::DidStartLoading(RenderViewHost* render_view_host) {
  SetIsLoading(true, NULL);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidStartLoading(render_view_host));
}
