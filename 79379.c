void ChangeListLoader::LoadAfterLoadDirectory(
    const DirectoryFetchInfo& directory_fetch_info,
    bool is_initial_load,
    int64 start_changestamp,
    FileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  OnDirectoryLoadComplete(directory_fetch_info, error);

  if (is_initial_load)
    LoadChangeListFromServer(start_changestamp);
}
