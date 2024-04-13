void RenderFrameImpl::didReceiveTitle(blink::WebLocalFrame* frame,
                                      const blink::WebString& title,
                                      blink::WebTextDirection direction) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->didReceiveTitle(frame, title, direction);
}
