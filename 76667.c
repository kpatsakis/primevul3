PpapiPluginProcessHost* PpapiPluginProcessHost::CreateBrokerHost(
    const content::PepperPluginInfo& info) {
  PpapiPluginProcessHost* plugin_host =
      new PpapiPluginProcessHost();
  if (plugin_host->Init(info))
    return plugin_host;

  NOTREACHED();  // Init is not expected to fail.
  return NULL;
}
