void NetworkActionPredictor::UpdateRow(
    DBCacheMap::iterator it,
    const NetworkActionPredictorDatabase::Row& row) {
  if (!initialized_)
    return;

  DCHECK(it != db_cache_.end());
  it->second.number_of_hits = row.number_of_hits;
  it->second.number_of_misses = row.number_of_misses;
  content::BrowserThread::PostTask(content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::UpdateRow, db_, row));
}
