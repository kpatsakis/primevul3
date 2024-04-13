void RenderView::OnPrintPages() {
  DCHECK(webview());
  if (webview()) {
    if (webview()->focusedFrame()->hasSelection())
      Print(webview()->focusedFrame(), false);
    else
      Print(webview()->mainFrame(), false);
  }
}
