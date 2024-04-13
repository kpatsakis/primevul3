int64 TypedUrlModelAssociator::GetSyncIdFromChromeId(
    const std::string& typed_url) {
  TypedUrlToSyncIdMap::const_iterator iter = id_map_.find(typed_url);
  return iter == id_map_.end() ? sync_api::kInvalidId : iter->second;
}
