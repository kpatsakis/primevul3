void DelegatedFrameHost::EvictDelegatedFrame() {
  client_->GetLayer()->SetShowPaintedContent();
  frame_provider_ = NULL;
  if (!surface_id_.is_null()) {
    surface_factory_->Destroy(surface_id_);
    surface_id_ = cc::SurfaceId();
  }
  delegated_frame_evictor_->DiscardedFrame();
}
