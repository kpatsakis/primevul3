void FileSystemOperation::Copy(const GURL& src_path_url,
                               const GURL& dest_path_url,
                               const StatusCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationCopy));

  base::PlatformFileError result = SetUpFileSystemPath(
      src_path_url, &src_path_, &src_util_, PATH_FOR_READ);
  if (result == base::PLATFORM_FILE_OK)
    result = SetUpFileSystemPath(
        dest_path_url, &dest_path_, &dest_util_, PATH_FOR_CREATE);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result);
    delete this;
    return;
  }

  GetUsageAndQuotaThenRunTask(
      dest_path_.origin(), dest_path_.type(),
      base::Bind(&FileSystemOperation::DoCopy,
                 base::Unretained(this), callback),
      base::Bind(callback, base::PLATFORM_FILE_ERROR_FAILED));
}
