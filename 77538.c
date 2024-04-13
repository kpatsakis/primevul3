void SyncManager::SyncInternal::SetPassphrase(
    const std::string& passphrase, bool is_explicit, bool user_provided) {
  DCHECK(user_provided || !is_explicit);
  if (passphrase.empty()) {
    DVLOG(1) << "Rejecting empty passphrase.";
    WriteTransaction trans(FROM_HERE, GetUserShare());
    Cryptographer* cryptographer = trans.GetCryptographer();
    sync_pb::EncryptedData pending_keys;
    if (cryptographer->has_pending_keys())
      pending_keys = cryptographer->GetPendingKeys();
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
        OnPassphraseRequired(sync_api::REASON_SET_PASSPHRASE_FAILED,
                             pending_keys));
    return;
  }

  WriteTransaction trans(FROM_HERE, GetUserShare());
  Cryptographer* cryptographer = trans.GetCryptographer();
  KeyParams key_params = {"localhost", "dummy", passphrase};

  WriteNode node(&trans);
  if (!node.InitByTagLookup(kNigoriTag)) {
    NOTREACHED();
    return;
  }
  bool nigori_has_explicit_passphrase =
      node.GetNigoriSpecifics().using_explicit_passphrase();

  std::string bootstrap_token;
  bool success = false;
  sync_pb::EncryptedData pending_keys;
  if (cryptographer->has_pending_keys()) {
    pending_keys = cryptographer->GetPendingKeys();
    success = SetDecryptionPassphrase(key_params,
                                      nigori_has_explicit_passphrase,
                                      is_explicit,
                                      user_provided,
                                      cryptographer,
                                      &bootstrap_token);
    if (success) {
      RequestNudge(FROM_HERE);
    }
  } else {
    success = SetEncryptionPassphrase(key_params,
                                      nigori_has_explicit_passphrase,
                                      is_explicit,
                                      cryptographer,
                                      &bootstrap_token);
  }

  if (!bootstrap_token.empty()) {
    DVLOG(1) << "Bootstrap token updated.";
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                      OnBootstrapTokenUpdated(bootstrap_token));
  }

  if (!success) {
    DVLOG(1) << "SetPassphrase failure, notifying and returning.";
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
    OnPassphraseRequired(sync_api::REASON_SET_PASSPHRASE_FAILED,
                         pending_keys));
    return;
  }
  DVLOG(1) << "SetPassphrase success, updating nigori and reencrypting.";
  FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                    OnPassphraseAccepted());
  DCHECK(cryptographer->is_ready());

  sync_pb::NigoriSpecifics specifics(node.GetNigoriSpecifics());
  if (!cryptographer->GetKeys(specifics.mutable_encrypted())) {
    NOTREACHED();
    return;
  }
  specifics.set_using_explicit_passphrase(is_explicit);
  node.SetNigoriSpecifics(specifics);

  ReEncryptEverything(&trans);
}
