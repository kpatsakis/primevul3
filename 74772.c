void RenderView::OnAdvanceToNextMisspelling() {
  if (!webview())
    return;
  webview()->focusedFrame()->executeCommand(
      WebString::fromUTF8("AdvanceToNextMisspelling"));
}
