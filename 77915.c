void RenderFrameImpl::didHandleOnloadEvents(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  if (!frame->parent())
    Send(new FrameHostMsg_DocumentOnLoadCompleted(routing_id_));
}
