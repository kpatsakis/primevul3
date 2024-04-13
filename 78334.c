CreateCommandBufferResult RenderThreadImpl::CreateViewCommandBuffer(
      int32 surface_id,
      const GPUCreateCommandBufferConfig& init_params,
      int32 route_id) {
  TRACE_EVENT1("gpu",
               "RenderThreadImpl::CreateViewCommandBuffer",
               "surface_id",
               surface_id);

  CreateCommandBufferResult result = CREATE_COMMAND_BUFFER_FAILED;
  IPC::Message* message = new GpuHostMsg_CreateViewCommandBuffer(
      surface_id,
      init_params,
      route_id,
      &result);

  thread_safe_sender()->Send(message);

  return result;
}
