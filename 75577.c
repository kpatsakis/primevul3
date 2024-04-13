void ExtensionService::SyncExtensionChangeIfNeeded(const Extension& extension) {
  SyncBundle* sync_bundle = GetSyncBundleForExtension(extension);
  if (sync_bundle) {
    ExtensionSyncData extension_sync_data(extension,
                                          IsExtensionEnabled(extension.id()),
                                          IsIncognitoEnabled(extension.id()));

    SyncChangeList sync_change_list(1, extension_sync_data.GetSyncChange(
        sync_bundle->HasExtensionId(extension.id()) ?
            SyncChange::ACTION_UPDATE : SyncChange::ACTION_ADD));
    sync_bundle->sync_processor->ProcessSyncChanges(
        FROM_HERE, sync_change_list);
    sync_bundle->synced_extensions.insert(extension.id());
    sync_bundle->pending_sync_data.erase(extension.id());
  }
}
