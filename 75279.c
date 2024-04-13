bool FilePathWatcherImpl::Watch(const FilePath& path,
                                FilePathWatcher::Delegate* delegate) {
  DCHECK(target_.empty());
  DCHECK(MessageLoopForIO::current());

  set_message_loop(base::MessageLoopProxy::current());
  delegate_ = delegate;
  target_ = path;
  MessageLoop::current()->AddDestructionObserver(this);

  std::vector<FilePath::StringType> comps;
  target_.GetComponents(&comps);
  DCHECK(!comps.empty());
  for (std::vector<FilePath::StringType>::const_iterator comp(++comps.begin());
       comp != comps.end(); ++comp) {
    watches_.push_back(WatchEntry(InotifyReader::kInvalidWatch, *comp));
  }
  watches_.push_back(WatchEntry(InotifyReader::kInvalidWatch,
                                FilePath::StringType()));
  return UpdateWatches();
}
