void FileSystemOperation::DidOpenFile(
    const OpenFileCallback& callback,
    base::PlatformFileError rv,
    base::PassPlatformFile file,
    bool unused) {
  if (rv == base::PLATFORM_FILE_OK)
    CHECK_NE(base::kNullProcessHandle, peer_handle_);
  callback.Run(rv, file.ReleaseValue(), peer_handle_);
}
