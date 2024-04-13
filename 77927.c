void RenderFrameImpl::forwardInputEvent(const blink::WebInputEvent* event) {
  Send(new FrameHostMsg_ForwardInputEvent(routing_id_, event));
}
