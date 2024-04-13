SyncDataList ExtensionService::GetAllSyncData(syncable::ModelType type) const {
  const SyncBundle* bundle = GetSyncBundleForModelTypeConst(type);
  CHECK(bundle);
  std::vector<ExtensionSyncData> extension_sync_data = GetSyncDataList(*bundle);
  SyncDataList result(extension_sync_data.size());
  for (int i = 0; i < static_cast<int>(extension_sync_data.size()); ++i) {
    result[i] = extension_sync_data[i].GetSyncData();
  }
  return result;
}
