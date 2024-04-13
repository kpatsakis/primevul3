  bool CanRequestURL(const GURL& url) {
    SchemeMap::const_iterator judgment(scheme_policy_.find(url.scheme()));

    if (judgment == scheme_policy_.end())
      return false;  // Unmentioned schemes are disallowed.

    return judgment->second;
  }
