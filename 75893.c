bool Extension::HasEffectiveAccessToAllHosts(
    const URLPatternSet& effective_host_permissions,
    const std::set<std::string>& api_permissions) {
  const URLPatternList patterns = effective_host_permissions.patterns();
  for (URLPatternList::const_iterator host = patterns.begin();
       host != patterns.end(); ++host) {
    if (host->match_all_urls() ||
        (host->match_subdomains() && host->host().empty()))
      return true;
  }

  return false;
}
