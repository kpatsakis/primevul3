NetworkActionPredictor::NetworkActionPredictor(Profile* profile)
    : profile_(profile),
      db_(new NetworkActionPredictorDatabase(profile)),
      initialized_(false) {
  content::BrowserThread::PostTask(content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::Initialize, db_));

  HistoryService* history_service =
      profile_->GetHistoryService(Profile::EXPLICIT_ACCESS);
  if (history_service)
    history_service->InMemoryDatabase();

  std::vector<NetworkActionPredictorDatabase::Row>* rows =
      new std::vector<NetworkActionPredictorDatabase::Row>();
  content::BrowserThread::PostTaskAndReply(
      content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::GetAllRows, db_, rows),
      base::Bind(&NetworkActionPredictor::CreateCaches, AsWeakPtr(),
                 base::Owned(rows)));

}
