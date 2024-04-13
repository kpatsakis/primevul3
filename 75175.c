void ChildProcessSecurityPolicy::GrantInspectElement(int renderer_id) {
  AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(renderer_id);
  if (state == security_state_.end())
    return;

  state->second->GrantScheme(chrome::kChromeUIScheme);
}
