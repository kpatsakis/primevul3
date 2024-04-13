bool FilePathWatcherImpl::UpdateWatches() {
  DCHECK(message_loop()->BelongsToCurrentThread());

  FilePath path(FILE_PATH_LITERAL("/"));
  bool path_valid = true;
  for (WatchVector::iterator watch_entry(watches_.begin());
       watch_entry != watches_.end(); ++watch_entry) {
    InotifyReader::Watch old_watch = watch_entry->watch_;
    if (path_valid) {
      watch_entry->watch_ = g_inotify_reader.Get().AddWatch(path, this);
      if ((watch_entry->watch_ == InotifyReader::kInvalidWatch) &&
          file_util::IsLink(path)) {
        FilePath link;
        file_util::ReadSymbolicLink(path, &link);
        if (!link.IsAbsolute())
          link = path.DirName().Append(link);
        watch_entry->watch_ =
            g_inotify_reader.Get().AddWatch(link.DirName(), this);
        if (watch_entry->watch_ != InotifyReader::kInvalidWatch) {
          watch_entry->linkname_ = link.BaseName().value();
        } else {
          DPLOG(WARNING) << "Watch failed for "  << link.DirName().value();
        }
      }
      if (watch_entry->watch_ == InotifyReader::kInvalidWatch) {
        path_valid = false;
      }
    } else {
      watch_entry->watch_ = InotifyReader::kInvalidWatch;
    }
    if (old_watch != InotifyReader::kInvalidWatch &&
        old_watch != watch_entry->watch_) {
      g_inotify_reader.Get().RemoveWatch(old_watch, this);
    }
    path = path.Append(watch_entry->subdir_);
  }

  return true;
}
