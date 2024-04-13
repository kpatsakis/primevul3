bool TypedUrlModelAssociator::DeleteAllNodes(
    sync_api::WriteTransaction* trans) {
  DCHECK(expected_loop_ == MessageLoop::current());
  for (TypedUrlToSyncIdMap::iterator node_id = id_map_.begin();
       node_id != id_map_.end(); ++node_id) {
    sync_api::WriteNode sync_node(trans);
    if (!sync_node.InitByIdLookup(node_id->second)) {
      LOG(ERROR) << "Typed url node lookup failed.";
      return false;
    }
    sync_node.Remove();
  }

  id_map_.clear();
  id_map_inverse_.clear();
  return true;
}
