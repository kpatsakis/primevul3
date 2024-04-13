bool PrintWebViewHelper::GetPrintFrame(WebKit::WebFrame** frame) {
  DCHECK(frame);
  DCHECK(render_view()->GetWebView());
  if (!render_view()->GetWebView())
    return false;

  *frame = render_view()->GetWebView()->focusedFrame()->hasSelection() ?
      render_view()->GetWebView()->focusedFrame() :
      render_view()->GetWebView()->mainFrame();
  return true;
}
