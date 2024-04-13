void RenderFrameImpl::didRunInsecureContent(
    blink::WebLocalFrame* frame,
    const blink::WebSecurityOrigin& origin,
    const blink::WebURL& target) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->Send(new ViewHostMsg_DidRunInsecureContent(
      render_view_->GetRoutingID(),
      origin.toString().utf8(),
      target));
}
