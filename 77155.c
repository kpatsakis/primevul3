void NetworkActionPredictor::DeleteRowsWithURLs(const std::set<GURL>& urls) {
  if (!initialized_)
    return;

  std::vector<NetworkActionPredictorDatabase::Row::Id> id_list;

  for (DBCacheMap::iterator it = db_cache_.begin(); it != db_cache_.end();) {
    if (urls.find(it->first.url) != urls.end()) {
      const DBIdCacheMap::iterator id_it = db_id_cache_.find(it->first);
      DCHECK(id_it != db_id_cache_.end());
      id_list.push_back(id_it->second);
      db_id_cache_.erase(id_it);
      db_cache_.erase(it++);
    } else {
      ++it;
    }
  }

  content::BrowserThread::PostTask(content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::DeleteRows, db_, id_list));
}
