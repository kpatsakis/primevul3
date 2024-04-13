void RenderView::didDestroyScriptContext(WebFrame* frame) {
  EventBindings::HandleContextDestroyed(frame);
}
