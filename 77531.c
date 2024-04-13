void SyncManager::SyncInternal::ReEncryptEverything(WriteTransaction* trans) {
  Cryptographer* cryptographer = trans->GetCryptographer();
  if (!cryptographer || !cryptographer->is_ready())
    return;
  syncable::ModelTypeSet encrypted_types = GetEncryptedTypes(trans);
  ModelSafeRoutingInfo routes;
  registrar_->GetModelSafeRoutingInfo(&routes);
  std::string tag;
  for (syncable::ModelTypeSet::Iterator iter = encrypted_types.First();
       iter.Good(); iter.Inc()) {
    if (iter.Get() == syncable::PASSWORDS ||
        iter.Get() == syncable::NIGORI ||
        routes.count(iter.Get()) == 0)
      continue;
    ReadNode type_root(trans);
    tag = syncable::ModelTypeToRootTag(iter.Get());
    if (!type_root.InitByTagLookup(tag)) {
      continue;
    }

    std::queue<int64> to_visit;
    int64 child_id = type_root.GetFirstChildId();
    to_visit.push(child_id);
    while (!to_visit.empty()) {
      child_id = to_visit.front();
      to_visit.pop();
      if (child_id == kInvalidId)
        continue;

      WriteNode child(trans);
      if (!child.InitByIdLookup(child_id)) {
        NOTREACHED();
        continue;
      }
      if (child.GetIsFolder()) {
        to_visit.push(child.GetFirstChildId());
      }
      if (child.GetEntry()->Get(syncable::UNIQUE_SERVER_TAG).empty()) {
        child.ResetFromSpecifics();
      }
      to_visit.push(child.GetSuccessorId());
    }
  }

  if (routes.count(syncable::PASSWORDS) > 0) {
    ReadNode passwords_root(trans);
    std::string passwords_tag =
        syncable::ModelTypeToRootTag(syncable::PASSWORDS);
    if (passwords_root.InitByTagLookup(passwords_tag)) {
      int64 child_id = passwords_root.GetFirstChildId();
      while (child_id != kInvalidId) {
        WriteNode child(trans);
        if (!child.InitByIdLookup(child_id)) {
          NOTREACHED();
          return;
        }
        child.SetPasswordSpecifics(child.GetPasswordSpecifics());
        child_id = child.GetSuccessorId();
      }
    }
  }

  FOR_EACH_OBSERVER(SyncManager::Observer, observers_, OnEncryptionComplete());
}
