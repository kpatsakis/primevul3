void ChildProcessSecurityPolicy::RegisterPseudoScheme(
    const std::string& scheme) {
  AutoLock lock(lock_);
  DCHECK(pseudo_schemes_.count(scheme) == 0) << "Add schemes at most once.";
  DCHECK(web_safe_schemes_.count(scheme) == 0) <<
      "Psuedo implies not web-safe.";

  pseudo_schemes_.insert(scheme);
}
