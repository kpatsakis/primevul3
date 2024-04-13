const Extension* ExtensionService::GetExtensionByWebExtent(const GURL& url) {
  for (size_t i = 0; i < extensions_.size(); ++i) {
    if (extensions_[i]->web_extent().MatchesURL(url))
      return extensions_[i];
  }
  return NULL;
}
