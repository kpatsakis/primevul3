void RenderView::OnCopy() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(WebString::fromUTF8("Copy"));
  UserMetricsRecordAction("Copy");
}
