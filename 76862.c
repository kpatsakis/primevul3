void FileSystemOperation::DoCopy(const StatusCallback& callback) {
  FileSystemFileUtilProxy::Copy(
      &operation_context_,
      src_util_, dest_util_,
      src_path_, dest_path_,
      base::Bind(&FileSystemOperation::DidFinishFileOperation,
                 base::Owned(this), callback));
}
