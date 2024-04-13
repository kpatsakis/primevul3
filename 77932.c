void RenderFrameImpl::initializeChildFrame(const blink::WebRect& frame_rect,
                                           float scale_factor) {
  Send(new FrameHostMsg_InitializeChildFrame(
      routing_id_, frame_rect, scale_factor));
}
