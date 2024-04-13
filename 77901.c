void RenderFrameImpl::didChangeScrollOffset(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->didChangeScrollOffset(frame);
}
