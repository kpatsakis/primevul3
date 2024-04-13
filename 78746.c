void DelegatedFrameHost::OnLostResources() {
  RenderWidgetHostImpl* host = client_->GetHost();
  if (frame_provider_.get() || !surface_id_.is_null())
    EvictDelegatedFrame();
  idle_frame_subscriber_textures_.clear();
  yuv_readback_pipeline_.reset();

  host->ScheduleComposite();
}
