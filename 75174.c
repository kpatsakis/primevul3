void ChildProcessSecurityPolicy::GrantDOMUIBindings(int renderer_id) {
  AutoLock lock(lock_);

  SecurityStateMap::iterator state = security_state_.find(renderer_id);
  if (state == security_state_.end())
    return;

  state->second->GrantBindings(BindingsPolicy::DOM_UI);

  state->second->GrantScheme(chrome::kChromeUIScheme);

  state->second->GrantScheme(chrome::kPrintScheme);

  state->second->GrantScheme(chrome::kFileScheme);
}
