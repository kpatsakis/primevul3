void WebContentsImpl::SetHistoryLengthAndPrune(
    const SiteInstance* site_instance,
    int history_length,
    int32 minimum_page_id) {
  if (render_manager_.pending_render_view_host()) {
    NOTREACHED();
    return;
  }
  RenderViewHostImpl* rvh = GetRenderViewHostImpl();
  if (!rvh) {
    NOTREACHED();
    return;
  }
  if (site_instance && rvh->GetSiteInstance() != site_instance) {
    NOTREACHED();
    return;
  }
  Send(new ViewMsg_SetHistoryLengthAndPrune(GetRoutingID(),
                                            history_length,
                                            minimum_page_id));
}
