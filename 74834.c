void RenderView::OnSetInitialFocus(bool reverse) {
  if (!webview())
    return;
  webview()->setInitialFocus(reverse);
}
