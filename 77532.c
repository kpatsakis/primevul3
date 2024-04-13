bool SyncManager::ReceivedExperimentalTypes(syncable::ModelTypeSet* to_add)
    const {
  ReadTransaction trans(FROM_HERE, GetUserShare());
  ReadNode node(&trans);
  if (!node.InitByTagLookup(kNigoriTag)) {
    DVLOG(1) << "Couldn't find Nigori node.";
    return false;
  }
  if (node.GetNigoriSpecifics().sync_tabs()) {
    to_add->Put(syncable::SESSIONS);
    return true;
  }
  return false;
}
