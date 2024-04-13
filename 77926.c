void RenderFrameImpl::didUpdateCurrentHistoryItem(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->didUpdateCurrentHistoryItem(frame);
}
