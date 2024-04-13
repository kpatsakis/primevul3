    ExtensionService::GetSyncBundleForExtensionSyncData(
    const ExtensionSyncData& extension_sync_data) {
  switch (extension_sync_data.type()) {
    case Extension::SYNC_TYPE_APP:
      return &app_sync_bundle_;
    case Extension::SYNC_TYPE_EXTENSION:
      return &extension_sync_bundle_;
    default:
      NOTREACHED();
      return NULL;
  }
}
