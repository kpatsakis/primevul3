void RenderView::didCreateScriptContext(WebFrame* frame) {
  EventBindings::HandleContextCreated(frame, false);
}
