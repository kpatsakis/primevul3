SessionStorageNamespace* WebContentsImpl::GetSessionStorageNamespace(
    SiteInstance* instance) {
  return controller_.GetSessionStorageNamespace(instance);
}
