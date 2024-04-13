void RenderView::OnMoveOrResizeStarted() {
  if (webview())
    webview()->hidePopups();
}
