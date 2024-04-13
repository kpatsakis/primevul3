void SyncManager::SyncInternal::HandleCalculateChangesChangeEventFromSyncer(
    const ImmutableWriteTransactionInfo& write_transaction_info,
    syncable::BaseTransaction* trans) {
  LOG_IF(WARNING, !ChangeBuffersAreEmpty()) <<
      "CALCULATE_CHANGES called with unapplied old changes.";

  Cryptographer* crypto = dir_manager()->GetCryptographer(trans);
  const syncable::ImmutableEntryKernelMutationMap& mutations =
      write_transaction_info.Get().mutations;
  for (syncable::EntryKernelMutationMap::const_iterator it =
           mutations.Get().begin(); it != mutations.Get().end(); ++it) {
    bool existed_before = !it->second.original.ref(syncable::IS_DEL);
    bool exists_now = !it->second.mutated.ref(syncable::IS_DEL);

    syncable::ModelType type =
        syncable::GetModelTypeFromSpecifics(
            it->second.mutated.ref(SPECIFICS));
    if (type < syncable::FIRST_REAL_MODEL_TYPE)
      continue;

    int64 handle = it->first;
    if (exists_now && !existed_before)
      change_buffers_[type].PushAddedItem(handle);
    else if (!exists_now && existed_before)
      change_buffers_[type].PushDeletedItem(handle);
    else if (exists_now && existed_before &&
             VisiblePropertiesDiffer(it->second, crypto)) {
      change_buffers_[type].PushUpdatedItem(
          handle, VisiblePositionsDiffer(it->second));
    }

    SetExtraChangeRecordData(handle, type, &change_buffers_[type], crypto,
                             it->second.original, existed_before, exists_now);
  }
}
