bool SharedMemory::ShareToProcessCommon(ProcessHandle process,
                                        SharedMemoryHandle *new_handle,
                                        bool close_self) {
  const int new_fd = dup(mapped_file_);
  if (new_fd < 0) {
    DPLOG(ERROR) << "dup() failed.";
    return false;
  }

  new_handle->fd = new_fd;
  new_handle->auto_close = true;

  if (close_self)
    Close();

  return true;
}
