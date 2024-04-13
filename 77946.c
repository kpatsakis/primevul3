void RenderFrameImpl::willReleaseScriptContext(blink::WebLocalFrame* frame,
                                               v8::Handle<v8::Context> context,
                                               int world_id) {
  DCHECK(!frame_ || frame_ == frame);

  FOR_EACH_OBSERVER(RenderFrameObserver,
                    observers_,
                    WillReleaseScriptContext(context, world_id));
}
