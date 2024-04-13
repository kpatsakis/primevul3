void DelegatedFrameHost::AddedToWindow() {
  ui::Compositor* compositor = client_->GetCompositor();
  if (compositor) {
    DCHECK(!vsync_manager_);
    vsync_manager_ = compositor->vsync_manager();
    vsync_manager_->AddObserver(this);
  }
}
