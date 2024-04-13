RenderFrameImpl::createContentDecryptionModule(
    blink::WebLocalFrame* frame,
    const blink::WebSecurityOrigin& security_origin,
    const blink::WebString& key_system) {
  DCHECK(!frame_ || frame_ == frame);
  return WebContentDecryptionModuleImpl::Create(
      frame, security_origin, key_system);
}
