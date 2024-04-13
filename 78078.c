SiteInstance* WebContentsImpl::GetSiteInstance() const {
  return render_manager_.current_host()->GetSiteInstance();
}
