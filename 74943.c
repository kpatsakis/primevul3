void RenderView::navigateBackForwardSoon(int offset) {
  Send(new ViewHostMsg_GoToEntryAtOffset(routing_id_, offset));
}
