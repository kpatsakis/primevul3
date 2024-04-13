void FileSystemOperation::DoCreateFile(
    const StatusCallback& callback,
    bool exclusive) {
  FileSystemFileUtilProxy::EnsureFileExists(
      &operation_context_,
      src_util_, src_path_,
      base::Bind(
          exclusive ? &FileSystemOperation::DidEnsureFileExistsExclusive
                    : &FileSystemOperation::DidEnsureFileExistsNonExclusive,
          base::Owned(this), callback));
}
