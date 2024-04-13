void RenderFrameImpl::didAbortLoading(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
#if defined(ENABLE_PLUGINS)
  if (frame != render_view_->webview()->mainFrame())
    return;
  PluginChannelHost::Broadcast(
      new PluginHostMsg_DidAbortLoading(render_view_->GetRoutingID()));
#endif
}
