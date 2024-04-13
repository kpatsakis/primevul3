void ChangeListLoader::LoadDirectoryFromServerAfterLoad(
    const DirectoryFetchInfo& directory_fetch_info,
    const FileOperationCallback& callback,
    FeedFetcher* fetcher,
    FileError error,
    ScopedVector<ChangeList> change_lists) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());
  DCHECK(!directory_fetch_info.empty());

  fast_fetch_feed_fetcher_set_.erase(fetcher);
  delete fetcher;

  if (error != FILE_ERROR_OK) {
    LOG(ERROR) << "Failed to load directory: "
               << directory_fetch_info.local_id()
               << ": " << FileErrorToString(error);
    callback.Run(error);
    return;
  }

  base::FilePath* directory_path = new base::FilePath;
  base::PostTaskAndReplyWithResult(
      blocking_task_runner_,
      FROM_HERE,
      base::Bind(&ChangeListProcessor::RefreshDirectory,
                 resource_metadata_,
                 directory_fetch_info,
                 base::Passed(&change_lists),
                 directory_path),
      base::Bind(&ChangeListLoader::LoadDirectoryFromServerAfterRefresh,
                 weak_ptr_factory_.GetWeakPtr(),
                 directory_fetch_info,
                 callback,
                 base::Owned(directory_path)));
}
