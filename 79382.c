void ChangeListLoader::LoadChangeListFromServerAfterUpdate(
    ChangeListProcessor* change_list_processor,
    bool should_notify_changed_directories,
    const base::Time& start_time,
    FileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  const base::TimeDelta elapsed = base::Time::Now() - start_time;
  util::Log(logging::LOG_INFO,
            "Change lists applied (elapsed time: %sms)",
            base::Int64ToString(elapsed.InMilliseconds()).c_str());

  if (should_notify_changed_directories) {
    for (std::set<base::FilePath>::iterator dir_iter =
            change_list_processor->changed_dirs().begin();
        dir_iter != change_list_processor->changed_dirs().end();
        ++dir_iter) {
      FOR_EACH_OBSERVER(ChangeListLoaderObserver, observers_,
                        OnDirectoryChanged(*dir_iter));
    }
  }

  OnChangeListLoadComplete(error);

  FOR_EACH_OBSERVER(ChangeListLoaderObserver,
                    observers_,
                    OnLoadFromServerComplete());
}
