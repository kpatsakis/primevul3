void FileSystemOperation::OpenFile(const GURL& path_url,
                                   int file_flags,
                                   base::ProcessHandle peer_handle,
                                   const OpenFileCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationOpenFile));
  scoped_ptr<FileSystemOperation> deleter(this);

  peer_handle_ = peer_handle;

  if (file_flags & (
      (base::PLATFORM_FILE_ENUMERATE | base::PLATFORM_FILE_TEMPORARY |
       base::PLATFORM_FILE_HIDDEN))) {
    callback.Run(base::PLATFORM_FILE_ERROR_FAILED,
                 base::PlatformFile(), base::ProcessHandle());
    return;
  }
  if (file_flags &
      (base::PLATFORM_FILE_CREATE | base::PLATFORM_FILE_OPEN_ALWAYS |
       base::PLATFORM_FILE_CREATE_ALWAYS | base::PLATFORM_FILE_OPEN_TRUNCATED |
       base::PLATFORM_FILE_WRITE | base::PLATFORM_FILE_EXCLUSIVE_WRITE |
       base::PLATFORM_FILE_DELETE_ON_CLOSE |
       base::PLATFORM_FILE_WRITE_ATTRIBUTES)) {
    base::PlatformFileError result = SetUpFileSystemPath(
        path_url, &src_path_, &src_util_, PATH_FOR_CREATE);
    if (result != base::PLATFORM_FILE_OK) {
      callback.Run(result, base::PlatformFile(), base::ProcessHandle());
      return;
    }
  } else {
    base::PlatformFileError result = SetUpFileSystemPath(
        path_url, &src_path_, &src_util_, PATH_FOR_READ);
    if (result != base::PLATFORM_FILE_OK) {
      callback.Run(result, base::PlatformFile(), base::ProcessHandle());
      return;
    }
  }
  GetUsageAndQuotaThenRunTask(
      src_path_.origin(), src_path_.type(),
      base::Bind(&FileSystemOperation::DoOpenFile,
                 base::Unretained(deleter.release()), callback, file_flags),
      base::Bind(callback, base::PLATFORM_FILE_ERROR_FAILED,
                 base::kInvalidPlatformFileValue,
                 base::kNullProcessHandle));
}
