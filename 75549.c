SyncError ExtensionService::MergeDataAndStartSyncing(
    syncable::ModelType type,
    const SyncDataList& initial_sync_data,
    SyncChangeProcessor* sync_processor) {
  CHECK(sync_processor);

  SyncBundle* bundle = NULL;

  switch (type) {
    case syncable::EXTENSIONS:
      bundle = &extension_sync_bundle_;
      bundle->filter = IsSyncableExtension;
      break;

    case syncable::APPS:
      bundle = &app_sync_bundle_;
      bundle->filter = IsSyncableApp;
      break;

    default:
      LOG(FATAL) << "Got " << type << " ModelType";
  }

  bundle->sync_processor = sync_processor;

  for (SyncDataList::const_iterator i = initial_sync_data.begin();
       i != initial_sync_data.end();
       ++i) {
    ExtensionSyncData extension_sync_data = ExtensionSyncData(*i);
    bundle->synced_extensions.insert(extension_sync_data.id());
    ProcessExtensionSyncData(extension_sync_data, *bundle);
  }

  SyncDataList sync_data_list = GetAllSyncData(type);
  SyncChangeList sync_change_list;
  for (SyncDataList::const_iterator i = sync_data_list.begin();
       i != sync_data_list.end();
       ++i) {
    if (bundle->HasExtensionId(i->GetTag()))
      sync_change_list.push_back(SyncChange(SyncChange::ACTION_UPDATE, *i));
    else
      sync_change_list.push_back(SyncChange(SyncChange::ACTION_ADD, *i));
  }
  bundle->sync_processor->ProcessSyncChanges(FROM_HERE, sync_change_list);

  return SyncError();
}
