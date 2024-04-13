bool SharedMemory::IsHandleValid(const SharedMemoryHandle& handle) {
  return handle.fd >= 0;
}
