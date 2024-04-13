void RenderView::didRunInsecureContent(
    WebFrame* frame, const WebSecurityOrigin& origin) {
  Send(new ViewHostMsg_DidRunInsecureContent(
      routing_id_,
      origin.toString().utf8()));
}
