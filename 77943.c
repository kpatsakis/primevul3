void RenderFrameImpl::willClose(blink::WebFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    FrameWillClose(frame));
}
