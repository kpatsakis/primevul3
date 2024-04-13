bool GpuProcessHostUIShim::OnControlMessageReceived(
    const IPC::Message& message) {
  DCHECK(CalledOnValidThread());

  IPC_BEGIN_MESSAGE_MAP(GpuProcessHostUIShim, message)
    IPC_MESSAGE_HANDLER(GpuHostMsg_OnLogMessage,
                        OnLogMessage)

    IPC_MESSAGE_HANDLER(GpuHostMsg_AcceleratedSurfaceBuffersSwapped,
                        OnAcceleratedSurfaceBuffersSwapped)
    IPC_MESSAGE_HANDLER(GpuHostMsg_AcceleratedSurfacePostSubBuffer,
                        OnAcceleratedSurfacePostSubBuffer)
    IPC_MESSAGE_HANDLER(GpuHostMsg_AcceleratedSurfaceSuspend,
                        OnAcceleratedSurfaceSuspend)
    IPC_MESSAGE_HANDLER(GpuHostMsg_GraphicsInfoCollected,
                        OnGraphicsInfoCollected)
    IPC_MESSAGE_HANDLER(GpuHostMsg_AcceleratedSurfaceNew,
                        OnAcceleratedSurfaceNew)
    IPC_MESSAGE_HANDLER(GpuHostMsg_AcceleratedSurfaceRelease,
                        OnAcceleratedSurfaceRelease)

#if defined(TOOLKIT_GTK) || defined(OS_WIN)
    IPC_MESSAGE_HANDLER(GpuHostMsg_ResizeView, OnResizeView)
#endif

    IPC_MESSAGE_UNHANDLED_ERROR()
  IPC_END_MESSAGE_MAP()

  return true;
}
