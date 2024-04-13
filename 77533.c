void SyncManager::SyncInternal::RefreshEncryption() {
  DCHECK(initialized_);

  WriteTransaction trans(FROM_HERE, GetUserShare());
  WriteNode node(&trans);
  if (!node.InitByTagLookup(kNigoriTag)) {
    NOTREACHED() << "Unable to set encrypted datatypes because Nigori node not "
                 << "found.";
    return;
  }

  Cryptographer* cryptographer = trans.GetCryptographer();

  if (!cryptographer->is_ready()) {
    DVLOG(1) << "Attempting to encrypt datatypes when cryptographer not "
             << "initialized, prompting for passphrase.";
    sync_pb::EncryptedData pending_keys;
    if (cryptographer->has_pending_keys())
      pending_keys = cryptographer->GetPendingKeys();
    FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                      OnPassphraseRequired(sync_api::REASON_DECRYPTION,
                                           pending_keys));
    return;
  }

  sync_pb::NigoriSpecifics nigori;
  nigori.CopyFrom(node.GetNigoriSpecifics());
  cryptographer->UpdateNigoriFromEncryptedTypes(&nigori);
  node.SetNigoriSpecifics(nigori);
  allstatus_.SetEncryptedTypes(cryptographer->GetEncryptedTypes());

  ReEncryptEverything(&trans);
}
