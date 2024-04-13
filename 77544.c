void SyncManager::SyncInternal::UpdateCryptographerAndNigori(
    const base::Callback<void(bool)>& done_callback) {
  DCHECK(initialized_);
  scoped_refptr<browser_sync::GetSessionNameTask> task =
      new browser_sync::GetSessionNameTask(base::Bind(
          &SyncManager::SyncInternal::UpdateCryptographerAndNigoriCallback,
          weak_ptr_factory_.GetWeakPtr(),
          done_callback));
  content::BrowserThread::PostTask(
      content::BrowserThread::FILE,
      FROM_HERE,
      base::Bind(&browser_sync::GetSessionNameTask::GetSessionNameAsync,
                 task.get()));
}
