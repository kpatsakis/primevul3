  void OnResourceListFetched(
      const FeedFetcherCallback& callback,
      google_apis::GDataErrorCode status,
      scoped_ptr<google_apis::ResourceList> resource_list) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
    DCHECK(!callback.is_null());

    FileError error = GDataToFileError(status);
    if (error != FILE_ERROR_OK) {
      callback.Run(error, ScopedVector<ChangeList>());
      return;
    }

    DCHECK(resource_list);
    ChangeList* change_list = new ChangeList(*resource_list);
    if (util::IsDriveV2ApiEnabled())
      FixResourceIdInChangeList(change_list);
    change_lists_.push_back(change_list);

    GURL next_url;
    if (resource_list->GetNextFeedURL(&next_url) && !next_url.is_empty()) {
      scheduler_->GetRemainingResourceList(
          next_url,
          base::Bind(&FastFetchFeedFetcher::OnResourceListFetched,
                     weak_ptr_factory_.GetWeakPtr(), callback));
      return;
    }

    UMA_HISTOGRAM_TIMES("Drive.DirectoryFeedLoadTime",
                        base::TimeTicks::Now() - start_time_);

    callback.Run(FILE_ERROR_OK, change_lists_.Pass());
  }
