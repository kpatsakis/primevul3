bool InitialSyncEndedForTypes(syncable::ModelTypeSet types,
                              sync_api::UserShare* share) {
  syncable::ScopedDirLookup lookup(share->dir_manager.get(),
                                   share->name);
  if (!lookup.good()) {
    DCHECK(false) << "ScopedDirLookup failed when checking initial sync";
    return false;
  }

  for (syncable::ModelTypeSet::Iterator i = types.First();
       i.Good(); i.Inc()) {
    if (!lookup->initial_sync_ended_for_type(i.Get()))
      return false;
  }
  return true;
}
