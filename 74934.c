void RenderView::focusPrevious() {
  Send(new ViewHostMsg_TakeFocus(routing_id_, true));
}
