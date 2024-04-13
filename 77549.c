history::InMemoryURLIndex* HistoryQuickProvider::GetIndex() {
  if (index_for_testing_.get())
    return index_for_testing_.get();

  HistoryService* const history_service =
      profile_->GetHistoryService(Profile::EXPLICIT_ACCESS);
  if (!history_service)
    return NULL;

  return history_service->InMemoryIndex();
}
