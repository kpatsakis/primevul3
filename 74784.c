void RenderView::OnCut() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(WebString::fromUTF8("Cut"));
  UserMetricsRecordAction("Cut");
}
