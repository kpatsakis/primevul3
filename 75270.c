void FilePathWatcherImpl::CancelOnMessageLoopThread() {
  if (!is_cancelled())
    set_cancelled();

  if (delegate_) {
    MessageLoop::current()->RemoveDestructionObserver(this);
    delegate_ = NULL;
  }

  for (WatchVector::iterator watch_entry(watches_.begin());
       watch_entry != watches_.end(); ++watch_entry) {
    if (watch_entry->watch_ != InotifyReader::kInvalidWatch)
      g_inotify_reader.Get().RemoveWatch(watch_entry->watch_, this);
  }
  watches_.clear();
  target_.clear();
}
