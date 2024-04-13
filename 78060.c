int32 WebContentsImpl::GetMaxPageID() {
  return GetMaxPageIDForSiteInstance(GetSiteInstance());
}
