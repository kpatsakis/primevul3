GpuChannelHost* RenderThreadImpl::EstablishGpuChannelSync(
    CauseForGpuLaunch cause_for_gpu_launch) {
  TRACE_EVENT0("gpu", "RenderThreadImpl::EstablishGpuChannelSync");

  if (gpu_channel_.get()) {
    if (!gpu_channel_->IsLost())
      return gpu_channel_.get();

    gpu_channel_ = NULL;
  }

  int client_id = 0;
  IPC::ChannelHandle channel_handle;
  gpu::GPUInfo gpu_info;
  if (!Send(new GpuHostMsg_EstablishGpuChannel(cause_for_gpu_launch,
                                               &client_id,
                                               &channel_handle,
                                               &gpu_info)) ||
#if defined(OS_POSIX)
      channel_handle.socket.fd == -1 ||
#endif
      channel_handle.name.empty()) {
    return NULL;
  }

  GetContentClient()->SetGpuInfo(gpu_info);

  io_message_loop_proxy_ = ChildProcess::current()->io_message_loop_proxy();

  gpu_channel_ = GpuChannelHost::Create(
      this, gpu_info, channel_handle,
      ChildProcess::current()->GetShutDownEvent());
  return gpu_channel_.get();
}
