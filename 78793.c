SharedMemoryHandle SharedMemory::handle() const {
  return FileDescriptor(mapped_file_, false);
}
