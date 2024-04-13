WebNavigationPolicy RenderView::decidePolicyForNavigation(
    WebFrame* frame, const WebURLRequest& request, WebNavigationType type,
    const WebNode&, WebNavigationPolicy default_policy, bool is_redirect) {
  const GURL& url = request.url();

  if (renderer_preferences_.browser_handles_top_level_requests &&
      IsNonLocalTopLevelNavigation(url, frame, type)) {
    last_top_level_navigation_page_id_ = page_id_;
    GURL referrer(request.httpHeaderField(WebString::fromUTF8("Referer")));
    OpenURL(url, referrer, default_policy);
    return WebKit::WebNavigationPolicyIgnore;  // Suppress the load here.
  }

  bool is_content_initiated =
      NavigationState::FromDataSource(frame->provisionalDataSource())->
          is_content_initiated();

  if (default_policy == WebKit::WebNavigationPolicyCurrentTab &&
      is_content_initiated && frame->parent() == NULL &&
      type != WebKit::WebNavigationTypeFormSubmitted &&
      !url.SchemeIs(chrome::kAboutScheme)) {
    if (CrossesExtensionExtents(frame, url) ||
        BindingsPolicy::is_dom_ui_enabled(enabled_bindings_) ||
        frame->isViewSourceModeEnabled() ||
        url.SchemeIs(chrome::kViewSourceScheme)) {
      OpenURL(url, GURL(), default_policy);
      return WebKit::WebNavigationPolicyIgnore;  // Suppress the load here.
    }

    if (BindingsPolicy::is_extension_enabled(enabled_bindings_) &&
        IsNonLocalTopLevelNavigation(url, frame, type)) {
        OpenURL(url, GURL(), default_policy);
        return WebKit::WebNavigationPolicyIgnore;  // Suppress the load here.
    }
  }

  bool is_fork =
      GURL(frame->url()) == GURL(chrome::kAboutBlankURL) &&
      historyBackListCount() < 1 &&
      historyForwardListCount() < 1 &&
      frame->opener() == NULL &&
      frame->parent() == NULL &&
      is_content_initiated &&
      default_policy == WebKit::WebNavigationPolicyCurrentTab &&
      type == WebKit::WebNavigationTypeOther;

  bool is_noreferrer_and_blank_target =
      frame->parent() == NULL &&
      frame->url().isEmpty() &&
      historyBackListCount() < 1 &&
      historyForwardListCount() < 1 &&
      request.httpHeaderField(WebString::fromUTF8("Referer")).isNull() &&
      opener_suppressed_ &&
      frame->opener() == NULL &&
      frame->name().isNull() &&
      !creator_url_.is_empty() &&
      is_content_initiated &&
      default_policy == WebKit::WebNavigationPolicyCurrentTab &&
      type == WebKit::WebNavigationTypeOther;

  if (is_fork || is_noreferrer_and_blank_target) {
    OpenURL(url, GURL(), default_policy);
    return WebKit::WebNavigationPolicyIgnore;
  }

  return default_policy;
}
