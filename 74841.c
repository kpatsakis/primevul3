void RenderView::OnStop() {
  if (webview())
    webview()->mainFrame()->stopLoading();
}
