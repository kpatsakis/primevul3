gfx::Vector2d RenderViewImpl::GetScrollOffset() {
  WebSize scroll_offset = webview()->mainFrame()->scrollOffset();
  return gfx::Vector2d(scroll_offset.width, scroll_offset.height);
}
