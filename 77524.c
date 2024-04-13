bool SyncManager::Init(
    const FilePath& database_location,
    const WeakHandle<JsEventHandler>& event_handler,
    const std::string& sync_server_and_path,
    int sync_server_port,
    bool use_ssl,
    HttpPostProviderFactory* post_factory,
    ModelSafeWorkerRegistrar* registrar,
    ChangeDelegate* change_delegate,
    const std::string& user_agent,
    const SyncCredentials& credentials,
    sync_notifier::SyncNotifier* sync_notifier,
    const std::string& restored_key_for_bootstrapping,
    bool setup_for_test_mode,
    UnrecoverableErrorHandler* unrecoverable_error_handler) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK(post_factory);
  DVLOG(1) << "SyncManager starting Init...";
  string server_string(sync_server_and_path);
  return data_->Init(database_location,
                     event_handler,
                     server_string,
                     sync_server_port,
                     use_ssl,
                     post_factory,
                     registrar,
                     change_delegate,
                     user_agent,
                     credentials,
                     sync_notifier,
                     restored_key_for_bootstrapping,
                     setup_for_test_mode,
                     unrecoverable_error_handler);
}
