bool TypedUrlModelAssociator::SyncModelHasUserCreatedNodes(bool* has_nodes) {
  DCHECK(has_nodes);
  *has_nodes = false;
  int64 typed_url_sync_id;
  if (!GetSyncIdForTaggedNode(kTypedUrlTag, &typed_url_sync_id)) {
    LOG(ERROR) << "Server did not create the top-level typed_url node. We "
               << "might be running against an out-of-date server.";
    return false;
  }
  sync_api::ReadTransaction trans(sync_service_->GetUserShare());

  sync_api::ReadNode typed_url_node(&trans);
  if (!typed_url_node.InitByIdLookup(typed_url_sync_id)) {
    LOG(ERROR) << "Server did not create the top-level typed_url node. We "
               << "might be running against an out-of-date server.";
    return false;
  }

  *has_nodes = sync_api::kInvalidId != typed_url_node.GetFirstChildId();
  return true;
}
