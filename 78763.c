void DelegatedFrameHost::WasShown() {
  delegated_frame_evictor_->SetVisible(true);

  if (!released_front_lock_.get()) {
    ui::Compositor* compositor = client_->GetCompositor();
    if (compositor)
      released_front_lock_ = compositor->GetCompositorLock();
  }
}
