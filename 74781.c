void RenderView::OnCopyImageAt(int x, int y) {
  webview()->copyImageAt(WebPoint(x, y));
}
