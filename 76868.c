void FileSystemOperation::GetMetadata(const GURL& path_url,
                                      const GetMetadataCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationGetMetadata));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_READ);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result, base::PlatformFileInfo(), FilePath());
    delete this;
    return;
  }

  FileSystemFileUtilProxy::GetFileInfo(
      &operation_context_, src_util_, src_path_,
      base::Bind(&FileSystemOperation::DidGetMetadata,
                 base::Owned(this), callback));
}
