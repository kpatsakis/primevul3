void FileSystemOperation::Write(
    const net::URLRequestContext* url_request_context,
    const GURL& path_url,
    const GURL& blob_url,
    int64 offset,
    const WriteCallback& callback) {
  DCHECK(SetPendingOperationType(kOperationWrite));

  base::PlatformFileError result = SetUpFileSystemPath(
      path_url, &src_path_, &src_util_, PATH_FOR_WRITE);
  if (result != base::PLATFORM_FILE_OK) {
    callback.Run(result, 0, false);
    delete this;
    return;
  }
  DCHECK(blob_url.is_valid());
  file_writer_delegate_.reset(new FileWriterDelegate(
          this, src_path_, offset));
  set_write_callback(callback);
  scoped_ptr<net::URLRequest> blob_request(
      new net::URLRequest(blob_url, file_writer_delegate_.get()));
  blob_request->set_context(url_request_context);

  GetUsageAndQuotaThenRunTask(
      src_path_.origin(), src_path_.type(),
      base::Bind(&FileSystemOperation::DoWrite, weak_factory_.GetWeakPtr(),
                 base::Passed(&blob_request)),
      base::Bind(callback, base::PLATFORM_FILE_ERROR_FAILED, 0, true));
}
