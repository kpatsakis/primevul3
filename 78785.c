bool SharedMemory::MapAt(off_t offset, size_t bytes) {
  if (mapped_file_ == -1)
    return false;

  if (bytes > static_cast<size_t>(std::numeric_limits<int>::max()))
    return false;

#if defined(OS_ANDROID)
  if (bytes == 0) {
    DCHECK_EQ(0, offset);
    int ashmem_bytes = ashmem_get_size_region(mapped_file_);
    if (ashmem_bytes < 0)
      return false;
    bytes = ashmem_bytes;
  }
#endif

  memory_ = mmap(NULL, bytes, PROT_READ | (read_only_ ? 0 : PROT_WRITE),
                 MAP_SHARED, mapped_file_, offset);

  bool mmap_succeeded = memory_ != (void*)-1 && memory_ != NULL;
  if (mmap_succeeded) {
    mapped_size_ = bytes;
    DCHECK_EQ(0U, reinterpret_cast<uintptr_t>(memory_) &
        (SharedMemory::MAP_MINIMUM_ALIGNMENT - 1));
  } else {
    memory_ = NULL;
  }

  return mmap_succeeded;
}
