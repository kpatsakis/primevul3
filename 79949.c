int WriteFileDescriptor(const int fd, const char* data, int size) {
  ssize_t bytes_written_total = 0;
  for (ssize_t bytes_written_partial = 0; bytes_written_total < size;
       bytes_written_total += bytes_written_partial) {
    bytes_written_partial =
        HANDLE_EINTR(write(fd, data + bytes_written_total,
                           size - bytes_written_total));
    if (bytes_written_partial < 0)
      return -1;
  }

  return bytes_written_total;
}
