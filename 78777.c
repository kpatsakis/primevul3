void SharedMemory::Close() {
  Unmap();

  if (mapped_file_ > 0) {
    if (HANDLE_EINTR(close(mapped_file_)) < 0)
      PLOG(ERROR) << "close";
    mapped_file_ = -1;
  }
}
