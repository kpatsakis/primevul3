void ChildProcessSecurityPolicyImpl::Remove(int child_id) {
  base::AutoLock lock(lock_);
  if (!security_state_.count(child_id))
    return;  // May be called multiple times.

  delete security_state_[child_id];
  security_state_.erase(child_id);
  worker_map_.erase(child_id);
}
