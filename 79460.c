PP_Resource PPB_Buffer_Proxy::AddProxyResource(
    const HostResource& resource,
    base::SharedMemoryHandle shm_handle,
    uint32_t size) {
  return (new Buffer(resource, shm_handle, size))->GetReference();
}
