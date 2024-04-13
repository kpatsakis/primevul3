void WebContentsImpl::DragSourceMovedTo(int client_x, int client_y,
                                        int screen_x, int screen_y) {
  if (browser_plugin_embedder_.get())
    browser_plugin_embedder_->DragSourceMovedTo(client_x, client_y,
                                                screen_x, screen_y);
  if (GetRenderViewHost())
    GetRenderViewHostImpl()->DragSourceMovedTo(client_x, client_y,
                                               screen_x, screen_y);
}
