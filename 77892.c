RenderFrameImpl::createWorkerPermissionClientProxy(
    blink::WebLocalFrame* frame) {
  if (!frame || !frame->view())
    return NULL;
  DCHECK(!frame_ || frame_ == frame);
  return GetContentClient()->renderer()->CreateWorkerPermissionClientProxy(
      this, frame);
}
