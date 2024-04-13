void DelegatedFrameHost::RemovingFromWindow() {
  RunOnCommitCallbacks();
  resize_lock_.reset();
  client_->GetHost()->WasResized();
  ui::Compositor* compositor = client_->GetCompositor();
  if (compositor && compositor->HasObserver(this))
    compositor->RemoveObserver(this);

  if (vsync_manager_) {
    vsync_manager_->RemoveObserver(this);
    vsync_manager_ = NULL;
  }
}
