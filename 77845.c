void RenderFrameImpl::OnCompositorFrameSwapped(const IPC::Message& message) {
  FrameMsg_CompositorFrameSwapped::Param param;
  if (!FrameMsg_CompositorFrameSwapped::Read(&message, &param))
    return;
  scoped_ptr<cc::CompositorFrame> frame(new cc::CompositorFrame);
  param.a.frame.AssignTo(frame.get());

  if (!compositing_helper_.get()) {
    compositing_helper_ =
        ChildFrameCompositingHelper::CreateCompositingHelperForRenderFrame(
            frame_, this, routing_id_);
    compositing_helper_->EnableCompositing(true);
  }
  compositing_helper_->OnCompositorFrameSwapped(frame.Pass(),
                                                param.a.producing_route_id,
                                                param.a.output_surface_id,
                                                param.a.producing_host_id,
                                                param.a.shared_memory_handle);
}
