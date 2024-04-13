int32 WebContentsImpl::GetMaxPageIDForSiteInstance(
    SiteInstance* site_instance) {
  if (max_page_ids_.find(site_instance->GetId()) == max_page_ids_.end())
    max_page_ids_[site_instance->GetId()] = -1;

  return max_page_ids_[site_instance->GetId()];
}
