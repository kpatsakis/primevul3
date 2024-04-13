  void OnChangeListFetched(
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
    change_lists_.push_back(new ChangeList(*resource_list));

    GURL next_url;
    if (resource_list->GetNextFeedURL(&next_url) && !next_url.is_empty()) {
      scheduler_->GetRemainingChangeList(
          next_url,
          base::Bind(&DeltaFeedFetcher::OnChangeListFetched,
                     weak_ptr_factory_.GetWeakPtr(), callback));
      return;
    }

    callback.Run(FILE_ERROR_OK, change_lists_.Pass());
  }
