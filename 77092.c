GpuProcessHostUIShim* GpuProcessHostUIShim::FromID(int host_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  return g_hosts_by_id.Pointer()->Lookup(host_id);
}
