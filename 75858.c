void NavigationController::LoadURL(const GURL& url, const GURL& referrer,
                                   PageTransition::Type transition) {
  needs_reload_ = false;

  NavigationEntry* entry = CreateNavigationEntry(url, referrer, transition,
                                                 profile_);

  LoadEntry(entry);
}
