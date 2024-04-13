const GURL& WebContentsImpl::GetURL() const {
  NavigationEntry* entry = controller_.GetVisibleEntry();
  return entry ? entry->GetVirtualURL() : GURL::EmptyGURL();
}
