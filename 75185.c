void ChildProcessSecurityPolicy::RegisterWebSafeScheme(
    const std::string& scheme) {
  AutoLock lock(lock_);
  DCHECK(web_safe_schemes_.count(scheme) == 0) << "Add schemes at most once.";
  DCHECK(pseudo_schemes_.count(scheme) == 0) << "Web-safe implies not psuedo.";

  web_safe_schemes_.insert(scheme);
}
