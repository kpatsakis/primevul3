void SyncManager::SyncInternal::UpdateCryptographerAndNigoriCallback(
    const base::Callback<void(bool)>& done_callback,
    const std::string& session_name) {
  syncable::ScopedDirLookup lookup(dir_manager(), username_for_share());
  if (!lookup.good()) {
    NOTREACHED()
        << "UpdateCryptographerAndNigori: lookup not good so bailing out";
    done_callback.Run(false);
    return;
  }
  if (!lookup->initial_sync_ended_for_type(syncable::NIGORI)) {
    done_callback.Run(false);  // Should only happen during first time sync.
    return;
  }

  bool success = false;
  {
    WriteTransaction trans(FROM_HERE, GetUserShare());
    Cryptographer* cryptographer = trans.GetCryptographer();
    WriteNode node(&trans);

    if (node.InitByTagLookup(kNigoriTag)) {
      sync_pb::NigoriSpecifics nigori(node.GetNigoriSpecifics());
      Cryptographer::UpdateResult result = cryptographer->Update(nigori);
      if (result == Cryptographer::NEEDS_PASSPHRASE) {
        sync_pb::EncryptedData pending_keys;
        if (cryptographer->has_pending_keys())
          pending_keys = cryptographer->GetPendingKeys();
        FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                          OnPassphraseRequired(sync_api::REASON_DECRYPTION,
                                               pending_keys));
      }

      if (!nigori.has_encrypted() && cryptographer->is_ready()) {
        if (!cryptographer->GetKeys(nigori.mutable_encrypted())) {
          NOTREACHED();
        }
      }

      chrome::VersionInfo version_info;
      bool contains_this_device = false;
      for (int i = 0; i < nigori.device_information_size(); ++i) {
        const sync_pb::DeviceInformation& device_information =
            nigori.device_information(i);
        if (device_information.cache_guid() == lookup->cache_guid()) {
          if (device_information.chrome_version() !=
              version_info.CreateVersionString()) {
            sync_pb::DeviceInformation* mutable_device_information =
                nigori.mutable_device_information(i);
            mutable_device_information->set_chrome_version(
                version_info.CreateVersionString());
          }
          contains_this_device = true;
        }
      }

      if (!contains_this_device) {
        sync_pb::DeviceInformation* device_information =
            nigori.add_device_information();
        device_information->set_cache_guid(lookup->cache_guid());
#if defined(OS_CHROMEOS)
        device_information->set_platform("ChromeOS");
#elif defined(OS_LINUX)
        device_information->set_platform("Linux");
#elif defined(OS_MACOSX)
        device_information->set_platform("Mac");
#elif defined(OS_WIN)
        device_information->set_platform("Windows");
#endif
        device_information->set_name(session_name);
        chrome::VersionInfo version_info;
        device_information->set_chrome_version(
            version_info.CreateVersionString());
      }

      cryptographer->UpdateNigoriFromEncryptedTypes(&nigori);
      node.SetNigoriSpecifics(nigori);

      allstatus_.SetCryptographerReady(cryptographer->is_ready());
      allstatus_.SetCryptoHasPendingKeys(cryptographer->has_pending_keys());
      allstatus_.SetEncryptedTypes(cryptographer->GetEncryptedTypes());

      success = cryptographer->is_ready();
    } else {
      NOTREACHED();
    }
  }

  done_callback.Run(success);
}
