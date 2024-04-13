bool ExtensionService::SyncBundle::HasPendingExtensionId(const std::string& id)
    const {
  return pending_sync_data.find(id) != pending_sync_data.end();
}
