WebNode PageClickTracker::GetFocusedNode() {
  WebView* web_view = render_view_->webview();
  if (!web_view)
    return WebNode();

  WebFrame* web_frame = web_view->focusedFrame();
  if (!web_frame)
    return WebNode();

  return web_frame->document().focusedNode();
}
