bool Extension::HasHostPermission(const GURL& url) const {
  for (URLPatternList::const_iterator host = host_permissions().begin();
       host != host_permissions().end(); ++host) {
    if (url.SchemeIs(chrome::kChromeUIScheme) &&
        url.host() != chrome::kChromeUIFaviconHost &&
        location() != Extension::COMPONENT)
      return false;

    if (host->MatchesURL(url))
      return true;
  }
  return false;
}
