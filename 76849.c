void FileSystemOperation::CreateFile(const GURL& path_url,
                                     bool exclusive,
                                     const StatusCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationCreateFile));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_CREATE);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result);
    delete this;
    return;
  }

  GetUsageAndQuotaThenRunTask(
      src_path_.origin(), src_path_.type(),
      base::Bind(&FileSystemOperation::DoCreateFile,
                 base::Unretained(this), callback, exclusive),
      base::Bind(callback, base::PLATFORM_FILE_ERROR_FAILED));
}
