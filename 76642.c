void ChildProcessSecurityPolicyImpl::RegisterWebSafeScheme(
    const std::string& scheme) {
  base::AutoLock lock(lock_);
  DCHECK(web_safe_schemes_.count(scheme) == 0) << "Add schemes at most once.";
  DCHECK(pseudo_schemes_.count(scheme) == 0) << "Web-safe implies not pseudo.";

  web_safe_schemes_.insert(scheme);
}
