void RenderViewImpl::InstrumentDidBeginFrame() {
  if (!webview())
    return;
  if (!webview()->devToolsAgent())
    return;
  webview()->devToolsAgent()->didComposite();
}
