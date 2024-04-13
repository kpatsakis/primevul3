PP_Resource PPB_Buffer_Proxy::CreateProxyResource(PP_Instance instance,
                                                  uint32_t size) {
  PluginDispatcher* dispatcher = PluginDispatcher::GetForInstance(instance);
  if (!dispatcher)
    return 0;

  HostResource result;
  ppapi::proxy::SerializedHandle shm_handle;
  dispatcher->Send(new PpapiHostMsg_PPBBuffer_Create(
      API_ID_PPB_BUFFER, instance, size, &result, &shm_handle));
  if (result.is_null() || !shm_handle.IsHandleValid() ||
      !shm_handle.is_shmem())
    return 0;

  return AddProxyResource(result, shm_handle.shmem(), size);
}
