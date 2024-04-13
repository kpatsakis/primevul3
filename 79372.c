void ChangeListLoader::CheckForUpdates(const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  if (IsRefreshing()) {
    pending_update_check_callback_ = callback;
    return;
  }

  if (loaded_) {
    util::Log(logging::LOG_INFO, "Checking for updates");
    Load(DirectoryFetchInfo(), callback);
  }
}
