void FileSystemOperation::DidEnsureFileExistsExclusive(
    const StatusCallback& callback,
    base::PlatformFileError rv, bool created) {
  if (rv == base::PLATFORM_FILE_OK && !created) {
    callback.Run(base::PLATFORM_FILE_ERROR_EXISTS);
  } else {
    DidFinishFileOperation(callback, rv);
  }
}
