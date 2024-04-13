void WebContentsImpl::OnAppCacheAccessed(const GURL& manifest_url,
                                         bool blocked_by_policy) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    AppCacheAccessed(manifest_url, blocked_by_policy));
}
