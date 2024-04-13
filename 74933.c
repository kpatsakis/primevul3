void RenderView::focusNext() {
  Send(new ViewHostMsg_TakeFocus(routing_id_, false));
}
