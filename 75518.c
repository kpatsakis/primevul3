void ExtensionService::GetSyncDataListHelper(
    const ExtensionList& extensions,
    const SyncBundle& bundle,
    std::vector<ExtensionSyncData>* sync_data_list) const {
  for (ExtensionList::const_iterator it = extensions.begin();
       it != extensions.end(); ++it) {
    const Extension& extension = **it;
    if (bundle.filter(extension) &&
        !bundle.HasPendingExtensionId(extension.id())) {
      sync_data_list->push_back(
          ExtensionSyncData(extension,
                            IsExtensionEnabled(extension.id()),
                            IsIncognitoEnabled(extension.id())));
    }
  }
}
