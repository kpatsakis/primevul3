bool ChildProcessSecurityPolicy::HasDOMUIBindings(int renderer_id) {
  AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(renderer_id);
  if (state == security_state_.end())
    return false;

  return state->second->has_dom_ui_bindings();
}
