void TypedUrlModelAssociator::Associate(
    const std::string* typed_url, int64 sync_id) {
  DCHECK(expected_loop_ == MessageLoop::current());
  DCHECK(!IsAssociated(*typed_url));
  DCHECK_NE(sync_api::kInvalidId, sync_id);
  DCHECK(id_map_inverse_.find(sync_id) == id_map_inverse_.end());
  id_map_[*typed_url] = sync_id;
  id_map_inverse_[sync_id] = *typed_url;
}
