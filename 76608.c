void ChildProcessSecurityPolicyImpl::AddChild(int child_id) {
  if (security_state_.count(child_id) != 0) {
    NOTREACHED() << "Add child process at most once.";
    return;
  }

  security_state_[child_id] = new SecurityState();
}
