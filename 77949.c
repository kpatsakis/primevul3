void RenderFrameImpl::willSendSubmitEvent(blink::WebLocalFrame* frame,
                                          const blink::WebFormElement& form) {
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    WillSendSubmitEvent(frame, form));
}
