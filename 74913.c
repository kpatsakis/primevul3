void RenderView::didExhaustMemoryAvailableForScript(WebFrame* frame) {
  Send(new ViewHostMsg_JSOutOfMemory(routing_id_));
}
