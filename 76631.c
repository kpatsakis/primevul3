void ChildProcessSecurityPolicyImpl::GrantScheme(int child_id,
                                                 const std::string& scheme) {
  base::AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return;

  state->second->GrantScheme(scheme);
}
