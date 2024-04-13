static bool UrlMatchesPermissions(
    const GURL& url, const std::vector<URLPattern>& host_permissions) {
  for (size_t i = 0; i < host_permissions.size(); ++i) {
    if (host_permissions[i].MatchesUrl(url))
      return true;
  }

  return false;
}
