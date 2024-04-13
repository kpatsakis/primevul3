void ChildProcessSecurityPolicy::GrantRequestURL(
    int renderer_id, const GURL& url) {

  if (!url.is_valid())
    return;  // Can't grant the capability to request invalid URLs.

  if (IsWebSafeScheme(url.scheme()))
    return;  // The scheme has already been white-listed for every renderer.

  if (IsPseudoScheme(url.scheme())) {
    if (url.SchemeIs(chrome::kViewSourceScheme) ||
        url.SchemeIs(chrome::kPrintScheme)) {
      GrantRequestURL(renderer_id, GURL(url.path()));
    }

    return;  // Can't grant the capability to request pseudo schemes.
  }

  {
    AutoLock lock(lock_);
    SecurityStateMap::iterator state = security_state_.find(renderer_id);
    if (state == security_state_.end())
      return;

    state->second->GrantScheme(url.scheme());
  }
}
