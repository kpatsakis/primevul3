void RenderView::OnClearFocusedNode() {
  if (webview())
    webview()->clearFocusedNode();
}
