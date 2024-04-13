void RenderView::OnSetBackground(const SkBitmap& background) {
  if (webview())
    webview()->setIsTransparent(!background.empty());

  SetBackground(background);
}
