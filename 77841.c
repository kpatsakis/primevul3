void RenderFrameImpl::OnBeforeUnload() {
  CHECK(!frame_->parent());

  base::TimeTicks before_unload_start_time = base::TimeTicks::Now();
  bool proceed = frame_->dispatchBeforeUnloadEvent();
  base::TimeTicks before_unload_end_time = base::TimeTicks::Now();
  Send(new FrameHostMsg_BeforeUnload_ACK(routing_id_, proceed,
                                         before_unload_start_time,
                                         before_unload_end_time));
}
