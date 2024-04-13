void RenderFrameImpl::frameDetached(blink::WebFrame* frame) {
  CHECK(!is_detaching_);
  DCHECK(!frame_ || frame_ == frame);

  bool is_subframe = !!frame->parent();

  Send(new FrameHostMsg_Detach(routing_id_));

  render_view_->UnregisterSwappedOutChildFrame(this);

  is_detaching_ = true;

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    FrameDetached(frame));

  FrameMap::iterator it = g_frame_map.Get().find(frame);
  CHECK(it != g_frame_map.Get().end());
  CHECK_EQ(it->second, this);
  g_frame_map.Get().erase(it);

  if (is_subframe)
    frame->parent()->removeChild(frame);

  frame->close();

  if (is_subframe) {
    delete this;
  }
}
