void ChangeListLoader::LoadDirectoryIfNeededAfterGetEntry(
    const base::FilePath& directory_path,
    const FileOperationCallback& callback,
    bool should_try_loading_parent,
    const ResourceEntry* entry,
    FileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  if (error == FILE_ERROR_NOT_FOUND &&
      should_try_loading_parent &&
      util::GetDriveGrandRootPath().IsParent(directory_path)) {
    LoadDirectoryIfNeeded(
        directory_path.DirName(),
        base::Bind(&ChangeListLoader::LoadDirectoryIfNeededAfterLoadParent,
                   weak_ptr_factory_.GetWeakPtr(),
                   directory_path,
                   callback));
    return;
  }
  if (error != FILE_ERROR_OK) {
    callback.Run(error);
    return;
  }

  if (!entry->file_info().is_directory()) {
    callback.Run(FILE_ERROR_NOT_A_DIRECTORY);
    return;
  }

  if (entry->local_id() == util::kDriveOtherDirLocalId) {
    callback.Run(FILE_ERROR_OK);
    return;
  }

  Load(DirectoryFetchInfo(entry->local_id(),
                          entry->resource_id(),
                          entry->directory_specific_info().changestamp()),
       callback);
}
