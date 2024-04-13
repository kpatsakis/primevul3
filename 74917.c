void RenderView::didLoadResourceFromMemoryCache(
    WebFrame* frame, const WebURLRequest& request,
    const WebURLResponse& response) {
  Send(new ViewHostMsg_DidLoadResourceFromMemoryCache(
      routing_id_,
      request.url(),
      frame->securityOrigin().toString().utf8(),
      frame->top()->securityOrigin().toString().utf8(),
      response.securityInfo()));
}
