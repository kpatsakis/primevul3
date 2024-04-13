void RenderView::didCreateIsolatedScriptContext(WebFrame* frame) {
  EventBindings::HandleContextCreated(frame, true);
}
