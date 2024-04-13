void ChangeListLoader::Load(const DirectoryFetchInfo& directory_fetch_info,
                            const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  const bool is_initial_load = (!loaded_ && !IsRefreshing());

  const std::string& local_id = directory_fetch_info.local_id();
  pending_load_callback_[local_id].push_back(callback);

  if (pending_load_callback_[local_id].size() > 1)
    return;

  if (is_initial_load && !directory_fetch_info.empty()) {
    pending_load_callback_[""].push_back(
        base::Bind(&util::EmptyFileOperationCallback));
  }

  base::PostTaskAndReplyWithResult(
      blocking_task_runner_,
      FROM_HERE,
      base::Bind(&ResourceMetadata::GetLargestChangestamp,
                 base::Unretained(resource_metadata_)),
      base::Bind(&ChangeListLoader::LoadAfterGetLargestChangestamp,
                 weak_ptr_factory_.GetWeakPtr(),
                 directory_fetch_info,
                 is_initial_load));
}
