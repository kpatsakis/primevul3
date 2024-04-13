RenderWidgetHostViewPort* WebContentsImpl::GetRenderWidgetHostViewPort() const {
  BrowserPluginGuest* guest = GetBrowserPluginGuest();
  if (guest && guest->embedder_web_contents()) {
    return guest->embedder_web_contents()->GetRenderWidgetHostViewPort();
  }
  return RenderWidgetHostViewPort::FromRWHV(GetRenderWidgetHostView());
}
