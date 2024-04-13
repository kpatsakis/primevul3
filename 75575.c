void ExtensionService::StopSyncing(syncable::ModelType type) {
  SyncBundle* bundle = GetSyncBundleForModelType(type);
  CHECK(bundle);
  *bundle = SyncBundle();
}
