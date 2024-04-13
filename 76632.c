void ChildProcessSecurityPolicyImpl::GrantWebUIBindings(int child_id) {
  base::AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(child_id);
  if (state == security_state_.end())
    return;

  state->second->GrantBindings(BINDINGS_POLICY_WEB_UI);

  state->second->GrantScheme(chrome::kChromeUIScheme);

  state->second->GrantScheme(chrome::kFileScheme);
}
