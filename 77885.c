blink::WebCookieJar* RenderFrameImpl::cookieJar(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  return &cookie_jar_;
}
