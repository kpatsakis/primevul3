void Extension::InitEffectiveHostPermissions() {
  if (HasApiPermission(api_permissions_, kProxyPermission) ||
      !devtools_url_.is_empty()) {
    URLPattern all_urls(URLPattern::SCHEME_ALL);
    all_urls.set_match_all_urls(true);
    effective_host_permissions_.AddPattern(all_urls);
    return;
  }

  for (URLPatternList::const_iterator host = host_permissions().begin();
       host != host_permissions().end(); ++host)
    effective_host_permissions_.AddPattern(*host);

  for (UserScriptList::const_iterator content_script =
           content_scripts().begin();
       content_script != content_scripts().end(); ++content_script) {
    URLPatternList::const_iterator pattern =
        content_script->url_patterns().begin();
    for (; pattern != content_script->url_patterns().end(); ++pattern)
      effective_host_permissions_.AddPattern(*pattern);
  }
}
