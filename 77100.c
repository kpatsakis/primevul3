void RouteToGpuProcessHostUIShimTask(int host_id, const IPC::Message& msg) {
  GpuProcessHostUIShim* ui_shim = GpuProcessHostUIShim::FromID(host_id);
  if (ui_shim)
    ui_shim->OnMessageReceived(msg);
}
