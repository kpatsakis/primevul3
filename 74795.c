void RenderView::OnEnableViewSourceMode() {
  if (!webview())
    return;
  WebFrame* main_frame = webview()->mainFrame();
  if (!main_frame)
    return;

  main_frame->enableViewSourceMode(true);
}
