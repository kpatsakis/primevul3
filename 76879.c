FileSystemOperation::~FileSystemOperation() {
  if (file_writer_delegate_.get()) {
    FileSystemOperationContext* c =
        new FileSystemOperationContext(operation_context_);
    base::FileUtilProxy::RelayClose(
        file_system_context()->file_task_runner(),
        base::Bind(&FileSystemFileUtil::Close,
                   base::Unretained(src_util_),
                   base::Owned(c)),
        file_writer_delegate_->file(),
        base::FileUtilProxy::StatusCallback());
  }
}
