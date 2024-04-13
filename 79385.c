void ChangeListLoader::LoadDirectoryFromServerAfterRefresh(
    const DirectoryFetchInfo& directory_fetch_info,
    const FileOperationCallback& callback,
    const base::FilePath* directory_path,
    FileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  DVLOG(1) << "Directory loaded: " << directory_fetch_info.ToString();
  callback.Run(error);
  if (error == FILE_ERROR_OK && !directory_path->empty()) {
    FOR_EACH_OBSERVER(ChangeListLoaderObserver, observers_,
                      OnDirectoryChanged(*directory_path));
  }
}
