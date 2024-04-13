void DelegatedFrameHost::OnCompositingLockStateChanged(
    ui::Compositor* compositor) {
  if (!compositor->IsLocked() && can_lock_compositor_ == YES_DID_LOCK) {
    can_lock_compositor_ = NO_PENDING_RENDERER_FRAME;
  }
}
