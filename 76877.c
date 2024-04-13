void FileSystemOperation::Truncate(const GURL& path_url, int64 length,
                                   const StatusCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationTruncate));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_WRITE);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result);
    delete this;
    return;
  }
  GetUsageAndQuotaThenRunTask(
      src_path_.origin(), src_path_.type(),
      base::Bind(&FileSystemOperation::DoTruncate,
                 base::Unretained(this), callback, length),
      base::Bind(callback, base::PLATFORM_FILE_ERROR_FAILED));
}
