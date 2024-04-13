bool SyncManager::SyncInternal::SetEncryptionPassphrase(
    const KeyParams& key_params,
    bool nigori_has_explicit_passphrase,
    bool is_explicit,
    Cryptographer* cryptographer,
    std::string *bootstrap_token) {
  if (cryptographer->has_pending_keys()) {
    LOG(ERROR) << "Attempt to set encryption passphrase failed because there "
               << "were pending keys.";
    return false;
  }
  if (!nigori_has_explicit_passphrase) {
    if (cryptographer->AddKey(key_params)) {
      DVLOG(1) << "Setting " << (is_explicit ? "explicit" : "implicit" )
               << " passphrase for encryption.";
      cryptographer->GetBootstrapToken(bootstrap_token);
      return true;
    } else {
      NOTREACHED() << "Failed to add key to cryptographer.";
      return false;
    }
  } else {  // nigori_has_explicit_passphrase == true
    NOTREACHED() << "Attempting to change explicit passphrase when one has "
                 << "already been set.";
    return false;
  }  // nigori_has_explicit_passphrase
  NOTREACHED();
  return false;
}
