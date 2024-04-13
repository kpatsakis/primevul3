Buffer::Buffer(const HostResource& resource,
               const base::SharedMemoryHandle& shm_handle,
               uint32_t size)
    : Resource(OBJECT_IS_PROXY, resource),
      shm_(shm_handle, false),
      size_(size),
      map_count_(0) {
}
