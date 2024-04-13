void InotifyReader::OnInotifyEvent(const inotify_event* event) {
  if (event->mask & IN_IGNORED)
    return;

  FilePath::StringType child(event->len ? event->name : FILE_PATH_LITERAL(""));
  base::AutoLock auto_lock(lock_);

  for (WatcherSet::iterator watcher = watchers_[event->wd].begin();
       watcher != watchers_[event->wd].end();
       ++watcher) {
    (*watcher)->OnFilePathChanged(event->wd,
                                  child,
                                  event->mask & (IN_CREATE | IN_MOVED_TO),
                                  event->mask & IN_ISDIR);
  }
}
