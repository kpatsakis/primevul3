void RenderView::OnRedo() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(WebString::fromUTF8("Redo"));
  UserMetricsRecordAction("Redo");
}
