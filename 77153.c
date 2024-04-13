void NetworkActionPredictor::DeleteOldEntries(history::URLDatabase* url_db) {
  CHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
  DCHECK(!initialized_);

  std::vector<NetworkActionPredictorDatabase::Row::Id> ids_to_delete;
  DeleteOldIdsFromCaches(url_db, &ids_to_delete);

  content::BrowserThread::PostTask(content::BrowserThread::DB, FROM_HERE,
      base::Bind(&NetworkActionPredictorDatabase::DeleteRows, db_,
                 ids_to_delete));

  notification_registrar_.Add(this, chrome::NOTIFICATION_OMNIBOX_OPENED_URL,
                              content::Source<Profile>(profile_));
  notification_registrar_.Add(this, chrome::NOTIFICATION_HISTORY_URLS_DELETED,
                              content::Source<Profile>(profile_));
  initialized_ = true;
}
