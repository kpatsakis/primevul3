void WebContentsImpl::DidRedirectProvisionalLoad(
    RenderViewHost* render_view_host,
    int32 page_id,
    const GURL& source_url,
    const GURL& target_url) {
  GURL validated_source_url(source_url);
  GURL validated_target_url(target_url);
  RenderProcessHost* render_process_host =
      render_view_host->GetProcess();
  RenderViewHost::FilterURL(render_process_host, false, &validated_source_url);
  RenderViewHost::FilterURL(render_process_host, false, &validated_target_url);
  NavigationEntry* entry;
  if (page_id == -1) {
    entry = controller_.GetPendingEntry();
  } else {
    entry = controller_.GetEntryWithPageID(render_view_host->GetSiteInstance(),
                                           page_id);
  }
  if (!entry || entry->GetURL() != validated_source_url)
    return;

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    ProvisionalChangeToMainFrameUrl(validated_target_url,
                                                    render_view_host));
}
