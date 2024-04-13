void RenderFrameImpl::didCreateScriptContext(blink::WebLocalFrame* frame,
                                             v8::Handle<v8::Context> context,
                                             int extension_group,
                                             int world_id) {
  DCHECK(!frame_ || frame_ == frame);
  GetContentClient()->renderer()->DidCreateScriptContext(
      frame, context, extension_group, world_id);
}
