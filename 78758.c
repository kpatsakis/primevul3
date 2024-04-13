bool DelegatedFrameHost::ShouldSkipFrame(gfx::Size size_in_dip) const {
  if (can_lock_compositor_ == NO_PENDING_RENDERER_FRAME ||
      can_lock_compositor_ == NO_PENDING_COMMIT ||
      !resize_lock_.get())
    return false;

  return size_in_dip != resize_lock_->expected_size();
}
