void RenderView::OnUndo() {
  if (!webview())
    return;

  webview()->focusedFrame()->executeCommand(WebString::fromUTF8("Undo"));
  UserMetricsRecordAction("Undo");
}
