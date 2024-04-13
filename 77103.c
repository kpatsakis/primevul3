void SendOnIOThreadTask(int host_id, IPC::Message* msg) {
  GpuProcessHost* host = GpuProcessHost::FromID(host_id);
  if (host)
    host->Send(msg);
  else
    delete msg;
}
