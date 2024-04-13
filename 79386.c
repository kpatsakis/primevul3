void ChangeListLoader::LoadDirectoryIfNeeded(
    const base::FilePath& directory_path,
    const FileOperationCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  if (loaded_ && !IsRefreshing()) {
    callback.Run(FILE_ERROR_OK);
    return;
  }

  ResourceEntry* entry = new ResourceEntry;
  base::PostTaskAndReplyWithResult(
      blocking_task_runner_.get(),
      FROM_HERE,
      base::Bind(&ResourceMetadata::GetResourceEntryByPath,
                 base::Unretained(resource_metadata_),
                 directory_path,
                 entry),
      base::Bind(&ChangeListLoader::LoadDirectoryIfNeededAfterGetEntry,
                 weak_ptr_factory_.GetWeakPtr(),
                 directory_path,
                 callback,
                 true,  // should_try_loading_parent
                 base::Owned(entry)));
}
