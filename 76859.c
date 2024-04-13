void FileSystemOperation::DidReadDirectory(
    const ReadDirectoryCallback& callback,
    base::PlatformFileError rv,
    const std::vector<base::FileUtilProxy::Entry>& entries,
    bool has_more) {
  callback.Run(rv, entries, has_more);
}
