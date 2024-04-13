void RenderView::OnPageTranslated() {
  WebFrame* frame = webview()->mainFrame();
  if (!frame)
    return;

  autofill_helper_.FrameContentsAvailable(frame);
}
