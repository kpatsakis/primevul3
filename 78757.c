bool DelegatedFrameHost::ShouldCreateResizeLock() {
  RenderWidgetHostImpl* host = client_->GetHost();

  if (resize_lock_)
    return false;

  if (host->should_auto_resize())
    return false;

  gfx::Size desired_size = client_->DesiredFrameSize();
  if (desired_size == current_frame_size_in_dip_ || desired_size.IsEmpty())
    return false;

  ui::Compositor* compositor = client_->GetCompositor();
  if (!compositor)
    return false;

  return true;
}
