blink::WebNavigationPolicy RenderFrameImpl::decidePolicyForNavigation(
    blink::WebLocalFrame* frame,
    blink::WebDataSource::ExtraData* extra_data,
    const blink::WebURLRequest& request,
    blink::WebNavigationType type,
    blink::WebNavigationPolicy default_policy,
    bool is_redirect) {
  DCHECK(!frame_ || frame_ == frame);
  return DecidePolicyForNavigation(
      this, frame, extra_data, request, type, default_policy, is_redirect);
}
