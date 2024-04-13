void NetworkActionPredictor::CommitTransaction() {
  if (!initialized_)
    return;

  content::BrowserThread::PostTask(content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::CommitTransaction, db_));
}
