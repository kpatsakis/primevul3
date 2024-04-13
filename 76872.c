void FileSystemOperation::Remove(const GURL& path_url, bool recursive,
                                 const StatusCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationRemove));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_WRITE);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result);
    delete this;
    return;
  }

  scoped_quota_notifier_.reset(new ScopedQuotaNotifier(
      file_system_context(), src_path_.origin(), src_path_.type()));

  FileSystemFileUtilProxy::Delete(
      &operation_context_, src_util_, src_path_, recursive,
      base::Bind(&FileSystemOperation::DidFinishFileOperation,
                 base::Owned(this), callback));
}
