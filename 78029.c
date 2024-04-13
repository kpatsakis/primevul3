void WebContentsImpl::DragSourceEndedAt(int client_x, int client_y,
    int screen_x, int screen_y, WebKit::WebDragOperation operation) {
  if (browser_plugin_embedder_.get())
    browser_plugin_embedder_->DragSourceEndedAt(client_x, client_y,
        screen_x, screen_y, operation);
  if (GetRenderViewHost())
    GetRenderViewHostImpl()->DragSourceEndedAt(client_x, client_y,
        screen_x, screen_y, operation);
}
