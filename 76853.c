void FileSystemOperation::DidEnsureFileExistsNonExclusive(
    const StatusCallback& callback,
    base::PlatformFileError rv, bool /* created */) {
  DidFinishFileOperation(callback, rv);
}
