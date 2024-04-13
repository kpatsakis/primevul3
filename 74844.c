void RenderView::OnToggleSpellCheck() {
  if (!webview())
    return;

  WebFrame* frame = webview()->focusedFrame();
  frame->enableContinuousSpellChecking(
      !frame->isContinuousSpellCheckingEnabled());
}
