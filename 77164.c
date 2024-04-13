bool NetworkActionPredictor::TryDeleteOldEntries(HistoryService* service) {
  if (!service)
    return false;

  history::URLDatabase* url_db = service->InMemoryDatabase();
  if (!url_db)
    return false;

  DeleteOldEntries(url_db);
  return true;
}
