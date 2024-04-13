void RenderFrameImpl::didDisplayInsecureContent(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->Send(new ViewHostMsg_DidDisplayInsecureContent(
      render_view_->GetRoutingID()));
}
