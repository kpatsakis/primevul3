bool ChildProcessSecurityPolicyImpl::CanAccessCookiesForOrigin(
    int child_id, const GURL& gurl) {
  base::AutoLock lock(lock_);
  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return false;
  return state->second->CanAccessCookiesForOrigin(gurl);
}
