const Extension* ExtensionService::GetExtensionByOverlappingWebExtent(
    const URLPatternSet& extent) {
  for (size_t i = 0; i < extensions_.size(); ++i) {
    if (extensions_[i]->web_extent().OverlapsWith(extent))
      return extensions_[i];
  }

  return NULL;
}
