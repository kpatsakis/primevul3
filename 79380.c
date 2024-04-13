void ChangeListLoader::LoadChangeListFromServer(int64 start_changestamp) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!change_feed_fetcher_);
  DCHECK(cached_about_resource_);

  bool is_delta_update = start_changestamp != 0;

  if (is_delta_update) {
    change_feed_fetcher_.reset(
        new DeltaFeedFetcher(scheduler_, start_changestamp));
  } else {
    change_feed_fetcher_.reset(new FullFeedFetcher(scheduler_));
  }

  change_feed_fetcher_->Run(
      base::Bind(&ChangeListLoader::LoadChangeListFromServerAfterLoadChangeList,
                 weak_ptr_factory_.GetWeakPtr(),
                 base::Passed(make_scoped_ptr(
                     new google_apis::AboutResource(*cached_about_resource_))),
                 is_delta_update));
}
