void ChangeListLoader::LoadChangeListFromServerAfterLoadChangeList(
    scoped_ptr<google_apis::AboutResource> about_resource,
    bool is_delta_update,
    FileError error,
    ScopedVector<ChangeList> change_lists) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(about_resource);

  change_feed_fetcher_.reset();

  if (error != FILE_ERROR_OK) {
    OnChangeListLoadComplete(error);
    return;
  }

  ChangeListProcessor* change_list_processor =
      new ChangeListProcessor(resource_metadata_);
  const bool should_notify_changed_directories = is_delta_update;

  util::Log(logging::LOG_INFO,
            "Apply change lists (is delta: %d)",
            is_delta_update);
  base::PostTaskAndReplyWithResult(
      blocking_task_runner_,
      FROM_HERE,
      base::Bind(&ChangeListProcessor::Apply,
                 base::Unretained(change_list_processor),
                 base::Passed(&about_resource),
                 base::Passed(&change_lists),
                 is_delta_update),
      base::Bind(&ChangeListLoader::LoadChangeListFromServerAfterUpdate,
                 weak_ptr_factory_.GetWeakPtr(),
                 base::Owned(change_list_processor),
                 should_notify_changed_directories,
                 base::Time::Now()));
}
