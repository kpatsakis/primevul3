void FileSystemOperation::DoWrite(scoped_ptr<net::URLRequest> blob_request) {
  int file_flags = base::PLATFORM_FILE_OPEN |
                   base::PLATFORM_FILE_WRITE |
                   base::PLATFORM_FILE_ASYNC;

  FileSystemOperationContext* write_context = new FileSystemOperationContext(
      operation_context_);
  FileSystemFileUtilProxy::CreateOrOpen(
      write_context, src_util_, src_path_, file_flags,
      base::Bind(&FileSystemOperation::OnFileOpenedForWrite,
                 weak_factory_.GetWeakPtr(),
                 base::Passed(&blob_request),
                 base::Owned(write_context)));
}
