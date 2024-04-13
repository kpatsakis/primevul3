bool InotifyReader::RemoveWatch(Watch watch,
                                FilePathWatcherImpl* watcher) {
  if (!valid_)
    return false;

  base::AutoLock auto_lock(lock_);

  watchers_[watch].erase(watcher);

  if (watchers_[watch].empty()) {
    watchers_.erase(watch);
    return (inotify_rm_watch(inotify_fd_, watch) == 0);
  }

  return true;
}
