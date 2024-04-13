void FileSystemOperation::DidTouchFile(const StatusCallback& callback,
                                       base::PlatformFileError rv) {
  callback.Run(rv);
}
