WebNavigationPolicy RenderFrameImpl::DecidePolicyForNavigation(
    RenderFrame* render_frame,
    WebFrame* frame,
    WebDataSource::ExtraData* extraData,
    const WebURLRequest& request,
    WebNavigationType type,
    WebNavigationPolicy default_policy,
    bool is_redirect) {
#ifdef OS_ANDROID
  if (request.url() != GURL(kSwappedOutURL) &&
      GetContentClient()->renderer()->HandleNavigation(
          render_frame,
          static_cast<DocumentState*>(extraData),
          render_view_->opener_id_,
          frame,
          request,
          type,
          default_policy,
          is_redirect)) {
    return blink::WebNavigationPolicyIgnore;
  }
#endif

  Referrer referrer(RenderViewImpl::GetReferrerFromRequest(frame, request));

  if (is_swapped_out_ || render_view_->is_swapped_out()) {
    if (request.url() != GURL(kSwappedOutURL)) {
      if (frame->parent() == NULL) {
        OpenURL(frame, request.url(), referrer, default_policy);
        return blink::WebNavigationPolicyIgnore;  // Suppress the load here.
      }

      return blink::WebNavigationPolicyIgnore;
    }

    return default_policy;
  }

  const GURL& url = request.url();

  bool is_content_initiated = static_cast<DocumentState*>(extraData)->
          navigation_state()->is_content_initiated();

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  bool force_swap_due_to_flag =
      command_line.HasSwitch(switches::kEnableStrictSiteIsolation) ||
      command_line.HasSwitch(switches::kSitePerProcess);
  if (force_swap_due_to_flag &&
      !frame->parent() && (is_content_initiated || is_redirect)) {
    WebString origin_str = frame->document().securityOrigin().toString();
    GURL frame_url(origin_str.utf8().data());
    bool same_domain_or_host =
        net::registry_controlled_domains::SameDomainOrHost(
            frame_url,
            url,
            net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);
    if (!same_domain_or_host || frame_url.scheme() != url.scheme()) {
      OpenURL(frame, url, referrer, default_policy);
      return blink::WebNavigationPolicyIgnore;
    }
  }

  if (is_content_initiated) {
    bool is_form_post = ((type == blink::WebNavigationTypeFormSubmitted) ||
                         (type == blink::WebNavigationTypeFormResubmitted)) &&
                        EqualsASCII(request.httpMethod(), "POST");
    bool browser_handles_request =
        render_view_->renderer_preferences_
            .browser_handles_non_local_top_level_requests
        && IsNonLocalTopLevelNavigation(url, frame, type, is_form_post);
    if (!browser_handles_request) {
      browser_handles_request = IsTopLevelNavigation(frame) &&
          render_view_->renderer_preferences_
              .browser_handles_all_top_level_requests;
    }

    if (browser_handles_request) {
      render_view_->page_id_ = -1;
      render_view_->last_page_id_sent_to_browser_ = -1;
      OpenURL(frame, url, referrer, default_policy);
      return blink::WebNavigationPolicyIgnore;  // Suppress the load here.
    }
  }

  GURL old_url(frame->dataSource()->request().url());

  if (!frame->parent() && is_content_initiated && !url.SchemeIs(kAboutScheme)) {
    bool send_referrer = false;

    int cumulative_bindings = RenderProcess::current()->GetEnabledBindings();
    bool is_initial_navigation = render_view_->page_id_ == -1;
    bool should_fork = HasWebUIScheme(url) || HasWebUIScheme(old_url) ||
        (cumulative_bindings & BINDINGS_POLICY_WEB_UI) ||
        url.SchemeIs(kViewSourceScheme) ||
        (frame->isViewSourceModeEnabled() &&
            type != blink::WebNavigationTypeReload);

    if (!should_fork && url.SchemeIs(kFileScheme)) {
      GURL source_url(old_url);
      if (is_initial_navigation && source_url.is_empty() && frame->opener())
        source_url = frame->opener()->top()->document().url();
      DCHECK(!source_url.is_empty());
      should_fork = !source_url.SchemeIs(kFileScheme);
    }

    if (!should_fork) {
      should_fork = GetContentClient()->renderer()->ShouldFork(
          frame, url, request.httpMethod().utf8(), is_initial_navigation,
          is_redirect, &send_referrer);
    }

    if (should_fork) {
      OpenURL(
          frame, url, send_referrer ? referrer : Referrer(), default_policy);
      return blink::WebNavigationPolicyIgnore;  // Suppress the load here.
    }
  }

  bool is_fork =
      old_url == GURL(kAboutBlankURL) &&
      render_view_->historyBackListCount() < 1 &&
      render_view_->historyForwardListCount() < 1 &&
      frame->opener() == NULL &&
      frame->parent() == NULL &&
      is_content_initiated &&
      default_policy == blink::WebNavigationPolicyCurrentTab &&
      type == blink::WebNavigationTypeOther;

  if (is_fork) {
    OpenURL(frame, url, Referrer(), default_policy);
    return blink::WebNavigationPolicyIgnore;
  }

  return default_policy;
}
