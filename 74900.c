void RenderView::didChangeScrollOffset(WebFrame* frame) {
  StartNavStateSyncTimerIfNecessary();
}
