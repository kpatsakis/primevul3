bool SyncManager::SyncInternal::OpenDirectory() {
  DCHECK(!initialized_) << "Should only happen once";

  change_observer_ =
      browser_sync::MakeWeakHandle(js_mutation_event_observer_.AsWeakPtr());

  bool share_opened =
      dir_manager()->Open(
          username_for_share(),
          this,
          unrecoverable_error_handler_,
          browser_sync::MakeWeakHandle(
              js_mutation_event_observer_.AsWeakPtr()));
  if (!share_opened) {
    LOG(ERROR) << "Could not open share for:" << username_for_share();
    return false;
  }

  syncable::ScopedDirLookup lookup(dir_manager(), username_for_share());
  if (!lookup.good()) {
    NOTREACHED();
    return false;
  }

  connection_manager()->set_client_id(lookup->cache_guid());
  return true;
}
