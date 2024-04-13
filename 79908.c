bool CopyFile(const FilePath& from_path, const FilePath& to_path) {
  base::ThreadRestrictions::AssertIOAllowed();
  int infile = HANDLE_EINTR(open(from_path.value().c_str(), O_RDONLY));
  if (infile < 0)
    return false;

  int outfile = HANDLE_EINTR(creat(to_path.value().c_str(), 0666));
  if (outfile < 0) {
    ignore_result(HANDLE_EINTR(close(infile)));
    return false;
  }

  const size_t kBufferSize = 32768;
  std::vector<char> buffer(kBufferSize);
  bool result = true;

  while (result) {
    ssize_t bytes_read = HANDLE_EINTR(read(infile, &buffer[0], buffer.size()));
    if (bytes_read < 0) {
      result = false;
      break;
    }
    if (bytes_read == 0)
      break;
    ssize_t bytes_written_per_read = 0;
    do {
      ssize_t bytes_written_partial = HANDLE_EINTR(write(
          outfile,
          &buffer[bytes_written_per_read],
          bytes_read - bytes_written_per_read));
      if (bytes_written_partial < 0) {
        result = false;
        break;
      }
      bytes_written_per_read += bytes_written_partial;
    } while (bytes_written_per_read < bytes_read);
  }

  if (HANDLE_EINTR(close(infile)) < 0)
    result = false;
  if (HANDLE_EINTR(close(outfile)) < 0)
    result = false;

  return result;
}
