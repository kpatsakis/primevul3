void SyncManager::RefreshNigori(const base::Closure& done_callback) {
  DCHECK(thread_checker_.CalledOnValidThread());
  data_->UpdateCryptographerAndNigori(base::Bind(
      &SyncManager::DoneRefreshNigori,
      base::Unretained(this),
      done_callback));
}
