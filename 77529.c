void SyncManager::SyncInternal::OnSyncEngineEvent(
    const SyncEngineEvent& event) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (event.what_happened == SyncEngineEvent::SYNC_CYCLE_ENDED) {
    ModelSafeRoutingInfo enabled_types;
    registrar_->GetModelSafeRoutingInfo(&enabled_types);
    {
      sync_api::ReadTransaction trans(FROM_HERE, GetUserShare());
      Cryptographer* cryptographer = trans.GetCryptographer();
      if (cryptographer->has_pending_keys()) {
        DVLOG(1) << "OnPassPhraseRequired Sent";
        sync_pb::EncryptedData pending_keys = cryptographer->GetPendingKeys();
        FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                          OnPassphraseRequired(sync_api::REASON_DECRYPTION,
                                               pending_keys));
      } else if (!cryptographer->is_ready() &&
                 event.snapshot->initial_sync_ended.Has(syncable::NIGORI)) {
        DVLOG(1) << "OnPassphraseRequired sent because cryptographer is not "
                 << "ready";
        FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                          OnPassphraseRequired(sync_api::REASON_ENCRYPTION,
                                               sync_pb::EncryptedData()));
      }

      allstatus_.SetCryptographerReady(cryptographer->is_ready());
      allstatus_.SetCryptoHasPendingKeys(cryptographer->has_pending_keys());
      allstatus_.SetEncryptedTypes(cryptographer->GetEncryptedTypes());
    }

    if (!initialized_) {
      LOG(INFO) << "OnSyncCycleCompleted not sent because sync api is not "
                << "initialized";
      return;
    }

    if (!event.snapshot->has_more_to_sync) {
      DVLOG(1) << "Sending OnSyncCycleCompleted";
      FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                        OnSyncCycleCompleted(event.snapshot));
    }

    bool is_notifiable_commit =
        (event.snapshot->syncer_status.num_successful_commits > 0);
    if (is_notifiable_commit) {
      if (sync_notifier_.get()) {
        const ModelTypeSet changed_types =
            syncable::ModelTypePayloadMapToEnumSet(
                event.snapshot->source.types);
        sync_notifier_->SendNotification(changed_types);
      } else {
        DVLOG(1) << "Not sending notification: sync_notifier_ is NULL";
      }
    }
  }

  if (event.what_happened == SyncEngineEvent::STOP_SYNCING_PERMANENTLY) {
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                      OnStopSyncingPermanently());
    return;
  }

  if (event.what_happened == SyncEngineEvent::CLEAR_SERVER_DATA_SUCCEEDED) {
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                      OnClearServerDataSucceeded());
    return;
  }

  if (event.what_happened == SyncEngineEvent::CLEAR_SERVER_DATA_FAILED) {
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                      OnClearServerDataFailed());
    return;
  }

  if (event.what_happened == SyncEngineEvent::UPDATED_TOKEN) {
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                      OnUpdatedToken(event.updated_token));
    return;
  }

  if (event.what_happened == SyncEngineEvent::ACTIONABLE_ERROR) {
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                      OnActionableError(
                          event.snapshot->errors.sync_protocol_error));
    return;
  }

}
