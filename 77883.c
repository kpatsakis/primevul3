bool RenderFrameImpl::allowWebGL(blink::WebLocalFrame* frame,
                                 bool default_value) {
  DCHECK(!frame_ || frame_ == frame);
  if (!default_value)
    return false;

  bool blocked = true;
  render_view_->Send(new ViewHostMsg_Are3DAPIsBlocked(
      render_view_->GetRoutingID(),
      GURL(frame->top()->document().securityOrigin().toString()),
      THREE_D_API_TYPE_WEBGL,
      &blocked));
  return !blocked;
}
