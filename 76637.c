bool ChildProcessSecurityPolicyImpl::IsDisabledScheme(
    const std::string& scheme) {
  base::AutoLock lock(lock_);
  return disabled_schemes_.find(scheme) != disabled_schemes_.end();
}
