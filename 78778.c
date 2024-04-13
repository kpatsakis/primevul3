void SharedMemory::CloseHandle(const SharedMemoryHandle& handle) {
  DCHECK_GE(handle.fd, 0);
  if (HANDLE_EINTR(close(handle.fd)) < 0)
    DPLOG(ERROR) << "close";
}
