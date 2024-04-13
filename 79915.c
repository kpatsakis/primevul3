bool CreateTemporaryFile(FilePath* path) {
  base::ThreadRestrictions::AssertIOAllowed();  // For call to close().
  FilePath directory;
  if (!GetTempDir(&directory))
    return false;
  int fd = CreateAndOpenFdForTemporaryFile(directory, path);
  if (fd < 0)
    return false;
  ignore_result(HANDLE_EINTR(close(fd)));
  return true;
}
