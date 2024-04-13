void RenderView::OnDragSourceEndedOrMoved(const gfx::Point& client_point,
                                          const gfx::Point& screen_point,
                                          bool ended,
                                          WebDragOperation op) {
  if (ended) {
    webview()->dragSourceEndedAt(client_point, screen_point, op);
  } else {
    webview()->dragSourceMovedTo(client_point, screen_point, op);
  }
}
