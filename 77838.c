void RenderFrameImpl::LoadURLExternally(blink::WebLocalFrame* frame,
                                        const blink::WebURLRequest& request,
                                        blink::WebNavigationPolicy policy) {
  DCHECK(!frame_ || frame_ == frame);
  loadURLExternally(frame, request, policy, WebString());
}
