void NetworkActionPredictor::AddRow(
    const DBCacheKey& key,
    const NetworkActionPredictorDatabase::Row& row) {
  if (!initialized_)
    return;

  DBCacheValue value = { row.number_of_hits, row.number_of_misses };
  db_cache_[key] = value;
  db_id_cache_[key] = row.id;
  content::BrowserThread::PostTask(content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::AddRow, db_, row));
}
