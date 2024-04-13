bool ChangeListLoader::IsRefreshing() const {
  return pending_load_callback_.find("") != pending_load_callback_.end();
}
