void RenderFrameImpl::frameFocused() {
  Send(new FrameHostMsg_FrameFocused(routing_id_));
}
