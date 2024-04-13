void WebContentsImpl::UpdateMaxPageIDIfNecessary(RenderViewHost* rvh) {
  int max_restored_page_id = controller_.GetMaxRestoredPageID();
  if (max_restored_page_id >
      GetMaxPageIDForSiteInstance(rvh->GetSiteInstance()))
    UpdateMaxPageIDForSiteInstance(rvh->GetSiteInstance(),
                                   max_restored_page_id);
}
