bool SyncManager::SyncInternal::SignIn(const SyncCredentials& credentials) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(share_.name.empty());
  share_.name = credentials.email;

  DVLOG(1) << "Signing in user: " << username_for_share();
  if (!OpenDirectory())
    return false;

  syncable::ScopedDirLookup lookup(dir_manager(), username_for_share());
  std::string unique_id;
  std::string state;
  if (lookup.good()) {
    unique_id = lookup->cache_guid();
    state = lookup->GetNotificationState();
    DVLOG(1) << "Read notification unique ID: " << unique_id;
    if (VLOG_IS_ON(1)) {
      std::string encoded_state;
      base::Base64Encode(state, &encoded_state);
      DVLOG(1) << "Read notification state: " << encoded_state;
    }
    allstatus_.SetUniqueId(unique_id);
  } else {
    LOG(ERROR) << "Could not read notification unique ID/state";
  }
  sync_notifier_->SetUniqueId(unique_id);
  sync_notifier_->SetState(state);

  UpdateCredentials(credentials);
  UpdateEnabledTypes();
  return true;
}
