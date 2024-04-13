void DelegatedFrameHost::CheckResizeLock() {
  if (!resize_lock_ ||
      resize_lock_->expected_size() != current_frame_size_in_dip_)
    return;

  resize_lock_->UnlockCompositor();
  ui::Compositor* compositor = client_->GetCompositor();
  if (compositor) {
    if (!compositor->HasObserver(this))
      compositor->AddObserver(this);
  }
}
