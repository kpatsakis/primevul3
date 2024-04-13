void FileSystemOperation::TouchFile(const GURL& path_url,
                                    const base::Time& last_access_time,
                                    const base::Time& last_modified_time,
                                    const StatusCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationTouchFile));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_WRITE);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result);
    delete this;
    return;
  }

  FileSystemFileUtilProxy::Touch(
      &operation_context_, src_util_, src_path_,
      last_access_time, last_modified_time,
      base::Bind(&FileSystemOperation::DidTouchFile,
                 base::Owned(this), callback));
}
