void RenderView::OnSelectAll() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(
      WebString::fromUTF8("SelectAll"));
  UserMetricsRecordAction("SelectAll");
}
