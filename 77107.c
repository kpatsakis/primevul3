GpuProcessHostUIShim::~GpuProcessHostUIShim() {
  DCHECK(CalledOnValidThread());
  g_hosts_by_id.Pointer()->Remove(host_id_);

  DictionaryValue* dict = new DictionaryValue();
  dict->SetInteger("level", logging::LOG_ERROR);
  dict->SetString("header", "GpuProcessHostUIShim");
  dict->SetString("message", "GPU Process Crashed.");
  GpuDataManagerImpl::GetInstance()->AddLogMessage(dict);
}
