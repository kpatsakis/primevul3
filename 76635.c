bool ChildProcessSecurityPolicyImpl::HasPermissionsForFileSystem(
    int child_id,
    const std::string& filesystem_id,
    int permission) {
  base::AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return false;
  return state->second->HasPermissionsForFileSystem(filesystem_id, permission);
}
