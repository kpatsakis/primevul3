void NetworkActionPredictor::DeleteAllRows() {
  if (!initialized_)
    return;

  db_cache_.clear();
  db_id_cache_.clear();
  content::BrowserThread::PostTask(content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::DeleteAllRows, db_));
}
