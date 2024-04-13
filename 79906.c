int AppendToFile(const FilePath& filename, const char* data, int size) {
  base::ThreadRestrictions::AssertIOAllowed();
  int fd = HANDLE_EINTR(open(filename.value().c_str(), O_WRONLY | O_APPEND));
  if (fd < 0)
    return -1;

  int bytes_written = WriteFileDescriptor(fd, data, size);
  if (int ret = HANDLE_EINTR(close(fd)) < 0)
    return ret;
  return bytes_written;
}
