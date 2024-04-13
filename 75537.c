static bool IsSyncableExtension(const Extension& extension) {
  return extension.GetSyncType() == Extension::SYNC_TYPE_EXTENSION;
}
