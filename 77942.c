bool RenderFrameImpl::willCheckAndDispatchMessageEvent(
    blink::WebLocalFrame* sourceFrame,
    blink::WebFrame* targetFrame,
    blink::WebSecurityOrigin targetOrigin,
    blink::WebDOMMessageEvent event) {
  DCHECK(!frame_ || frame_ == targetFrame);
  return render_view_->willCheckAndDispatchMessageEvent(
      sourceFrame, targetFrame, targetOrigin, event);
}
