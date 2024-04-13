void RenderView::OnDragTargetDrop(const gfx::Point& client_point,
                                  const gfx::Point& screen_point) {
  webview()->dragTargetDrop(client_point, screen_point);
}
