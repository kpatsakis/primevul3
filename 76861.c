void FileSystemOperation::DidWrite(
    base::PlatformFileError rv,
    int64 bytes,
    bool complete) {
  if (write_callback_.is_null()) {
    delete this;
    return;
  }
  write_callback_.Run(rv, bytes, complete);
  if (complete || rv != base::PLATFORM_FILE_OK)
    delete this;
}
