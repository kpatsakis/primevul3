void ChildProcessSecurityPolicyImpl::LockToOrigin(int child_id,
                                                  const GURL& gurl) {
  DCHECK(SiteInstanceImpl::GetSiteForURL(NULL, gurl) == gurl);
  base::AutoLock lock(lock_);
  SecurityStateMap::iterator state = security_state_.find(child_id);
  DCHECK(state != security_state_.end());
  state->second->LockToOrigin(gurl);
}
