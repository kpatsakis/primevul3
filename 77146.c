void NetworkActionPredictor::BeginTransaction() {
  if (!initialized_)
    return;

  content::BrowserThread::PostTask(content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::BeginTransaction, db_));
}
