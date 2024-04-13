void ChildProcessSecurityPolicyImpl::GrantPermissionsForFileSystem(
    int child_id,
    const std::string& filesystem_id,
    int permission) {
  base::AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return;
  state->second->GrantPermissionsForFileSystem(filesystem_id, permission);
}
