void DelegatedFrameHost::UnlockResources() {
  DCHECK(frame_provider_ || !surface_id_.is_null());
  delegated_frame_evictor_->UnlockFrame();
}
