void ChildProcessSecurityPolicyImpl::RegisterDisabledSchemes(
    const std::set<std::string>& schemes) {
  base::AutoLock lock(lock_);
  disabled_schemes_ = schemes;
}
