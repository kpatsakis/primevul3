scoped_ptr<base::SharedMemory> RenderThreadImpl::AllocateSharedMemory(
    size_t size) {
  return scoped_ptr<base::SharedMemory>(
      HostAllocateSharedMemoryBuffer(size));
}
