void SyncManager::DoneRefreshNigori(const base::Closure& done_callback,
                                    bool is_ready) {
  if (is_ready)
    data_->RefreshEncryption();
  done_callback.Run();
}
