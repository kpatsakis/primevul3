const GURL& WebContentsImpl::GetVisibleURL() const {
  NavigationEntry* entry = controller_.GetVisibleEntry();
  return entry ? entry->GetVirtualURL() : GURL::EmptyGURL();
}
