void FileSystemOperation::ReadDirectory(const GURL& path_url,
                                        const ReadDirectoryCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationReadDirectory));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_READ);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result, std::vector<base::FileUtilProxy::Entry>(), false);
    delete this;
    return;
  }

  FileSystemFileUtilProxy::ReadDirectory(
      &operation_context_, src_util_, src_path_,
      base::Bind(&FileSystemOperation::DidReadDirectory,
                 base::Owned(this), callback));
}
