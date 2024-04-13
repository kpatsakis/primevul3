void NetworkActionPredictor::DeleteOldIdsFromCaches(
    history::URLDatabase* url_db,
    std::vector<NetworkActionPredictorDatabase::Row::Id>* id_list) {
  CHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
  DCHECK(url_db);
  DCHECK(id_list);
  id_list->clear();
  for (DBCacheMap::iterator it = db_cache_.begin(); it != db_cache_.end();) {
    history::URLRow url_row;

    if ((url_db->GetRowForURL(it->first.url, &url_row) == 0) ||
        ((base::Time::Now() - url_row.last_visit()).InDays() >
         kMaximumDaysToKeepEntry)) {
      const DBIdCacheMap::iterator id_it = db_id_cache_.find(it->first);
      DCHECK(id_it != db_id_cache_.end());
      id_list->push_back(id_it->second);
      db_id_cache_.erase(id_it);
      db_cache_.erase(it++);
    } else {
      ++it;
    }
  }
}
