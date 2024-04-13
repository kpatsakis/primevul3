void ChangeListLoader::LoadDirectoryFromServer(
    const DirectoryFetchInfo& directory_fetch_info,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());
  DCHECK(!directory_fetch_info.empty());
  DCHECK(cached_about_resource_);
  DVLOG(1) << "Start loading directory: " << directory_fetch_info.ToString();

  if (directory_fetch_info.local_id() == util::kDriveGrandRootLocalId) {
    base::FilePath* changed_directory_path = new base::FilePath;
    base::PostTaskAndReplyWithResult(
        blocking_task_runner_,
        FROM_HERE,
        base::Bind(&AddMyDriveIfNeeded,
                   resource_metadata_,
                   google_apis::AboutResource(*cached_about_resource_),
                   changed_directory_path),
        base::Bind(&ChangeListLoader::LoadDirectoryFromServerAfterRefresh,
                   weak_ptr_factory_.GetWeakPtr(),
                   directory_fetch_info,
                   callback,
                   base::Owned(changed_directory_path)));
    return;
  }

  FastFetchFeedFetcher* fetcher = new FastFetchFeedFetcher(
      scheduler_,
      drive_service_,
      directory_fetch_info.resource_id(),
      cached_about_resource_->root_folder_id());
  fast_fetch_feed_fetcher_set_.insert(fetcher);
  fetcher->Run(
      base::Bind(&ChangeListLoader::LoadDirectoryFromServerAfterLoad,
                 weak_ptr_factory_.GetWeakPtr(),
                 directory_fetch_info,
                 callback,
                 fetcher));
}
