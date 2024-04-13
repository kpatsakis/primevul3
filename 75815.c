 ProfileSyncService* GetSyncService(Profile* profile) {
   if (test_sync_service)
    return test_sync_service;
  else
    return profile->GetProfileSyncService();
}
