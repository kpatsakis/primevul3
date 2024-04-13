bool ChildProcessSecurityPolicyImpl::CanReadRawCookies(int child_id) {
  base::AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return false;

  return state->second->can_read_raw_cookies();
}
