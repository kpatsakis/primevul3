void RenderFrameImpl::didLoseWebGLContext(blink::WebLocalFrame* frame,
                                          int arb_robustness_status_code) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->Send(new ViewHostMsg_DidLose3DContext(
      GURL(frame->top()->document().securityOrigin().toString()),
      THREE_D_API_TYPE_WEBGL,
      arb_robustness_status_code));
}
