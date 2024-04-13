void DelegatedFrameHost::MaybeCreateResizeLock() {
  if (!client_->ShouldCreateResizeLock())
    return;
  DCHECK(client_->GetCompositor());

  ui::Compositor* compositor = client_->GetCompositor();
  if (!compositor->HasObserver(this))
    compositor->AddObserver(this);

  bool defer_compositor_lock =
      can_lock_compositor_ == NO_PENDING_RENDERER_FRAME ||
      can_lock_compositor_ == NO_PENDING_COMMIT;

  if (can_lock_compositor_ == YES_CAN_LOCK)
    can_lock_compositor_ = YES_DID_LOCK;

  resize_lock_ = client_->CreateResizeLock(defer_compositor_lock);
}
