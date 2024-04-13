void FileSystemOperation::DoTruncate(const StatusCallback& callback,
                                     int64 length) {
  FileSystemFileUtilProxy::Truncate(
      &operation_context_, src_util_, src_path_, length,
      base::Bind(&FileSystemOperation::DidFinishFileOperation,
                 base::Owned(this), callback));
}
