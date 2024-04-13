void WebContentsImpl::DidSendScreenRects(RenderWidgetHostImpl* rwh) {
  if (browser_plugin_embedder_)
    browser_plugin_embedder_->DidSendScreenRects();
}
