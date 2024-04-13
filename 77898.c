void RenderFrameImpl::didChangeIcon(blink::WebLocalFrame* frame,
                                    blink::WebIconURL::Type icon_type) {
  DCHECK(!frame_ || frame_ == frame);
  render_view_->didChangeIcon(frame, icon_type);
}
