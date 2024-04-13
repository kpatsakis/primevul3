void DelegatedFrameHost::OnCompositingDidCommit(
    ui::Compositor* compositor) {
  RenderWidgetHostImpl* host = client_->GetHost();
  if (can_lock_compositor_ == NO_PENDING_COMMIT) {
    can_lock_compositor_ = YES_CAN_LOCK;
    if (resize_lock_.get() && resize_lock_->GrabDeferredLock())
      can_lock_compositor_ = YES_DID_LOCK;
  }
  RunOnCommitCallbacks();
  if (resize_lock_ &&
      resize_lock_->expected_size() == current_frame_size_in_dip_) {
    resize_lock_.reset();
    host->WasResized();
    MaybeCreateResizeLock();
  }
}
