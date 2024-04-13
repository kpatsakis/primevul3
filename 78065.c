SiteInstance* WebContentsImpl::GetPendingSiteInstance() const {
  RenderViewHost* dest_rvh = render_manager_.pending_render_view_host() ?
      render_manager_.pending_render_view_host() :
      render_manager_.current_host();
  return dest_rvh->GetSiteInstance();
}
