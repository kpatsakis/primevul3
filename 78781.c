size_t SharedMemory::GetHandleLimit() {
  return base::GetMaxFds();
}
