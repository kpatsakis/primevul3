void ChildProcessSecurityPolicy::Remove(int renderer_id) {
  AutoLock lock(lock_);
  if (!security_state_.count(renderer_id))
    return;  // May be called multiple times.

  delete security_state_[renderer_id];
  security_state_.erase(renderer_id);
}
