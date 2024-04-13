static bool IsSyncableApp(const Extension& extension) {
  return extension.GetSyncType() == Extension::SYNC_TYPE_APP;
}
