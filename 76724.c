void CompositorLock::CancelLock() {
  if (!root_window_)
    return;
  root_window_->UnlockCompositor();
  root_window_ = NULL;
}
