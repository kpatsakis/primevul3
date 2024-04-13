RenderWidgetFullscreenPepper* RenderFrameImpl::CreatePepperFullscreenContainer(
    PepperPluginInstanceImpl* plugin) {
  GURL active_url;
  if (render_view_->webview() && render_view_->webview()->mainFrame())
    active_url = GURL(render_view_->webview()->mainFrame()->document().url());
  RenderWidgetFullscreenPepper* widget = RenderWidgetFullscreenPepper::Create(
      GetRenderWidget()->routing_id(), plugin, active_url,
      GetRenderWidget()->screenInfo());
  widget->show(blink::WebNavigationPolicyIgnore);
  return widget;
}
