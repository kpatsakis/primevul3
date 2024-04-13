void RenderViewImpl::InstrumentWillComposite() {
  if (!webview())
    return;
  if (!webview()->devToolsAgent())
    return;
  webview()->devToolsAgent()->willComposite();
}
