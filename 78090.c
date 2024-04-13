WebPreferences WebContentsImpl::GetWebkitPrefs() {
  GURL url = controller_.GetActiveEntry()
      ? controller_.GetActiveEntry()->GetURL() : GURL::EmptyGURL();
  return GetWebkitPrefs(GetRenderViewHost(), url);
}
