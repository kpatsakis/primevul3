TypedUrlModelAssociator::TypedUrlModelAssociator(
    ProfileSyncService* sync_service,
    history::HistoryBackend* history_backend)
    : sync_service_(sync_service),
      history_backend_(history_backend),
      typed_url_node_id_(sync_api::kInvalidId),
      expected_loop_(MessageLoop::current()) {
  DCHECK(sync_service_);
  DCHECK(history_backend_);
  DCHECK(!BrowserThread::CurrentlyOn(BrowserThread::UI));
}
