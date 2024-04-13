std::vector<ExtensionSyncData> ExtensionService::GetSyncDataList(
    const SyncBundle& bundle) const {
  std::vector<ExtensionSyncData> extension_sync_list;
  GetSyncDataListHelper(extensions_, bundle, &extension_sync_list);
  GetSyncDataListHelper(disabled_extensions_, bundle, &extension_sync_list);
  GetSyncDataListHelper(terminated_extensions_, bundle, &extension_sync_list);

  for (std::map<std::string, ExtensionSyncData>::const_iterator i =
           bundle.pending_sync_data.begin();
       i != bundle.pending_sync_data.end();
       ++i) {
    extension_sync_list.push_back(i->second);
  }

  return extension_sync_list;
}
