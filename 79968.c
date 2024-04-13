void RenderViewImpl::Close() {
  WebView* doomed = webview();
  RenderWidget::Close();
  g_view_map.Get().erase(doomed);
  g_routing_id_view_map.Get().erase(routing_id_);
}
