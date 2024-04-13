void RenderView::didDisplayInsecureContent(WebFrame* frame) {
  Send(new ViewHostMsg_DidDisplayInsecureContent(routing_id_));
}
