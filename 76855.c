void FileSystemOperation::DidFinishFileOperation(
    const StatusCallback& callback,
    base::PlatformFileError rv) {
  if (!cancel_callback_.is_null()) {
    DCHECK_EQ(kOperationTruncate, pending_operation_);

    callback.Run(base::PLATFORM_FILE_ERROR_ABORT);
    cancel_callback_.Run(base::PLATFORM_FILE_OK);
    cancel_callback_.Reset();
  } else {
    callback.Run(rv);
  }
}
