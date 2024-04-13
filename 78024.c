void WebContentsImpl::DidStartProvisionalLoadForFrame(
    RenderViewHost* render_view_host,
    int64 frame_id,
    int64 parent_frame_id,
    bool is_main_frame,
    const GURL& url) {
  bool is_error_page = (url.spec() == kUnreachableWebDataURL);
  bool is_iframe_srcdoc = (url.spec() == kAboutSrcDocURL);
  GURL validated_url(url);
  RenderProcessHost* render_process_host =
      render_view_host->GetProcess();
  RenderViewHost::FilterURL(render_process_host, false, &validated_url);

  if (is_main_frame) {
    DidChangeLoadProgress(0);

    NavigationEntry* pending_entry = controller_.GetPendingEntry();
    bool has_browser_initiated_pending_entry = pending_entry &&
        !NavigationEntryImpl::FromNavigationEntry(pending_entry)->
            is_renderer_initiated();
    if (!has_browser_initiated_pending_entry && !is_error_page) {
      NavigationEntryImpl* entry = NavigationEntryImpl::FromNavigationEntry(
          controller_.CreateNavigationEntry(validated_url,
                                            content::Referrer(),
                                            content::PAGE_TRANSITION_LINK,
                                            true /* is_renderer_initiated */,
                                            std::string(),
                                            GetBrowserContext()));
      entry->set_site_instance(
          static_cast<SiteInstanceImpl*>(GetSiteInstance()));
      controller_.SetPendingEntry(entry);
      NotifyNavigationStateChanged(content::INVALIDATE_TYPE_URL);
    }
  }

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidStartProvisionalLoadForFrame(frame_id, parent_frame_id,
                    is_main_frame, validated_url, is_error_page,
                    is_iframe_srcdoc, render_view_host));

  if (is_main_frame) {
    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      ProvisionalChangeToMainFrameUrl(validated_url,
                                                      render_view_host));
  }
}
