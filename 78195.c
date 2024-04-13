void WebContentsImpl::UpdateMaxPageID(int32 page_id) {
  UpdateMaxPageIDForSiteInstance(GetSiteInstance(), page_id);
}
