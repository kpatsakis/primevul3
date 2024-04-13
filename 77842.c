void RenderFrameImpl::OnBuffersSwapped(
    const FrameMsg_BuffersSwapped_Params& params) {
  if (!compositing_helper_.get()) {
    compositing_helper_ =
        ChildFrameCompositingHelper::CreateCompositingHelperForRenderFrame(
            frame_, this, routing_id_);
    compositing_helper_->EnableCompositing(true);
  }
  compositing_helper_->OnBuffersSwapped(
      params.size,
      params.mailbox,
      params.gpu_route_id,
      params.gpu_host_id,
      render_view_->GetWebView()->deviceScaleFactor());
}
