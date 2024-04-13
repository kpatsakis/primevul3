bool SharedMemory::Unmap() {
  if (memory_ == NULL)
    return false;

  munmap(memory_, mapped_size_);
  memory_ = NULL;
  mapped_size_ = 0;
  return true;
}
