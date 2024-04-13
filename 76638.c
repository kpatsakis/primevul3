bool ChildProcessSecurityPolicyImpl::IsWebSafeScheme(
    const std::string& scheme) {
  base::AutoLock lock(lock_);

  return (web_safe_schemes_.find(scheme) != web_safe_schemes_.end());
}
