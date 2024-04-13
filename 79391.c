void ChangeListLoader::OnDirectoryLoadComplete(
    const DirectoryFetchInfo& directory_fetch_info,
    FileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  util::Log(logging::LOG_INFO,
            "Fast-fetch complete: %s => %s",
            directory_fetch_info.ToString().c_str(),
            FileErrorToString(error).c_str());
  const std::string& local_id = directory_fetch_info.local_id();
  LoadCallbackMap::iterator it = pending_load_callback_.find(local_id);
  if (it != pending_load_callback_.end()) {
    DVLOG(1) << "Running callback for " << local_id;
    const std::vector<FileOperationCallback>& callbacks = it->second;
    for (size_t i = 0; i < callbacks.size(); ++i) {
      base::MessageLoopProxy::current()->PostTask(
          FROM_HERE,
          base::Bind(callbacks[i], error));
    }
    pending_load_callback_.erase(it);
  }
}
