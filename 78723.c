void DelegatedFrameHost::AddOnCommitCallbackAndDisableLocks(
    const base::Closure& callback) {
  ui::Compositor* compositor = client_->GetCompositor();
  DCHECK(compositor);

  if (!compositor->HasObserver(this))
    compositor->AddObserver(this);

  can_lock_compositor_ = NO_PENDING_COMMIT;
  on_compositing_did_commit_callbacks_.push_back(callback);
}
