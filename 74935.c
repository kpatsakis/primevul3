void RenderView::focusedNodeChanged(const WebNode& node) {
  Send(new ViewHostMsg_FocusedNodeChanged(routing_id_));
}
