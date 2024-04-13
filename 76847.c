void FileSystemOperation::Cancel(const StatusCallback& cancel_callback) {
  if (file_writer_delegate_.get()) {
    DCHECK_EQ(kOperationWrite, pending_operation_);

    const bool delete_now = file_writer_delegate_->Cancel();

    if (!write_callback_.is_null()) {
      write_callback_.Run(base::PLATFORM_FILE_ERROR_ABORT, 0, false);
    }
    cancel_callback.Run(base::PLATFORM_FILE_OK);
    write_callback_.Reset();

    if (delete_now) {
      delete this;
      return;
    }
  } else {
    DCHECK_EQ(kOperationTruncate, pending_operation_);
    DCHECK(cancel_callback_.is_null());
    cancel_callback_ = cancel_callback;
  }
}
