const GURL& WebContentsImpl::GetLastCommittedURL() const {
  NavigationEntry* entry = controller_.GetLastCommittedEntry();
  return entry ? entry->GetVirtualURL() : GURL::EmptyGURL();
}
