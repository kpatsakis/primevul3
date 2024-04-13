syncable::ModelTypeSet GetTypesWithEmptyProgressMarkerToken(
    syncable::ModelTypeSet types,
    sync_api::UserShare* share) {
  syncable::ScopedDirLookup lookup(share->dir_manager.get(),
                                   share->name);
  if (!lookup.good()) {
    NOTREACHED() << "ScopedDirLookup failed for "
                  << "GetTypesWithEmptyProgressMarkerToken";
    return syncable::ModelTypeSet();
  }

  syncable::ModelTypeSet result;
  for (syncable::ModelTypeSet::Iterator i = types.First();
       i.Good(); i.Inc()) {
    sync_pb::DataTypeProgressMarker marker;
    lookup->GetDownloadProgress(i.Get(), &marker);

    if (marker.token().empty())
      result.Put(i.Get());

  }
  return result;
}
