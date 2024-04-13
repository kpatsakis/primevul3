bool SyncManager::SyncInternal::SetDecryptionPassphrase(
    const KeyParams& key_params,
    bool nigori_has_explicit_passphrase,
    bool is_explicit,
    bool user_provided,
    Cryptographer* cryptographer,
    std::string *bootstrap_token) {
  if (!cryptographer->has_pending_keys()) {
    NOTREACHED() << "Attempt to set decryption passphrase failed because there "
                 << "were no pending keys.";
    return false;

  }
  if (!nigori_has_explicit_passphrase) {
    if (!is_explicit) {
      if (!user_provided) {
        if (cryptographer->DecryptPendingKeys(key_params)) {
          DVLOG(1) << "Implicit internal passphrase accepted for decryption.";
          cryptographer->GetBootstrapToken(bootstrap_token);
          return true;
        } else {
          DVLOG(1) << "Implicit internal passphrase failed to decrypt, adding "
                   << "anyways as default passphrase and persisting via "
                   << "bootstrap token.";
          Cryptographer temp_cryptographer;
          temp_cryptographer.AddKey(key_params);
          temp_cryptographer.GetBootstrapToken(bootstrap_token);
          cryptographer->AddKey(key_params);
          return false;
        }
      } else {  // user_provided == true
        if (cryptographer->is_initialized()) {
          Cryptographer temp_cryptographer;
          temp_cryptographer.SetPendingKeys(cryptographer->GetPendingKeys());
          if (temp_cryptographer.DecryptPendingKeys(key_params)) {
            sync_pb::EncryptedData encrypted;
            cryptographer->GetKeys(&encrypted);
            if (temp_cryptographer.CanDecrypt(encrypted)) {
              DVLOG(1) << "Implicit user provided passphrase accepted for "
                       << "decryption, overwriting default.";
              cryptographer->DecryptPendingKeys(key_params);
              cryptographer->GetBootstrapToken(bootstrap_token);
              return true;
            } else {
              DVLOG(1) << "Implicit user provided passphrase accepted for "
                       << "decryption, restoring implicit internal passphrase "
                       << "as default.";
              std::string bootstrap_token_from_current_key;
              cryptographer->GetBootstrapToken(
                  &bootstrap_token_from_current_key);
              cryptographer->DecryptPendingKeys(key_params);
              cryptographer->AddKeyFromBootstrapToken(
                  bootstrap_token_from_current_key);
              return true;
            }
          }
        } else if (cryptographer->DecryptPendingKeys(key_params)) {
          DVLOG(1) << "Implicit user provided passphrase accepted, initializing"
                   << " cryptographer.";
          return true;
        } else {
          DVLOG(1) << "Implicit user provided passphrase failed to decrypt.";
          return false;
        }
      }  // user_provided
    } else {  // is_explicit == true
      DVLOG(1) << "Explicit passphrase failed to decrypt because nigori had "
               << "implicit passphrase.";
      return false;
    }  // is_explicit
  } else {  // nigori_has_explicit_passphrase == true
    if (!is_explicit) {
      DVLOG(1) << "Implicit passphrase rejected because nigori had explicit "
               << "passphrase.";
      return false;
    } else {  // is_explicit == true
      if (cryptographer->DecryptPendingKeys(key_params)) {
        DVLOG(1) << "Explicit passphrase accepted for decryption.";
        cryptographer->GetBootstrapToken(bootstrap_token);
        return true;
      } else {
        DVLOG(1) << "Explicit passphrase failed to decrypt.";
        return false;
      }
    }  // is_explicit
  }  // nigori_has_explicit_passphrase
  NOTREACHED();
  return false;
}
