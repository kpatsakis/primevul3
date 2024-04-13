SyncError ExtensionService::ProcessSyncChanges(
    const tracked_objects::Location& from_here,
    const SyncChangeList& change_list) {
  for (SyncChangeList::const_iterator i = change_list.begin();
      i != change_list.end();
      ++i) {
    ExtensionSyncData extension_sync_data = ExtensionSyncData(*i);
    SyncBundle* bundle = GetSyncBundleForExtensionSyncData(extension_sync_data);
    CHECK(bundle);

    if (extension_sync_data.uninstalled())
      bundle->synced_extensions.erase(extension_sync_data.id());
    else
      bundle->synced_extensions.insert(extension_sync_data.id());
    ProcessExtensionSyncData(extension_sync_data, *bundle);
  }

  return SyncError();
}
