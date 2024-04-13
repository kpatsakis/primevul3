void RenderViewImpl::InstrumentWillBeginFrame() {
  if (!webview())
    return;
  if (!webview()->devToolsAgent())
    return;
  webview()->devToolsAgent()->didBeginFrame();
}
