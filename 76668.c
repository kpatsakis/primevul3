PpapiPluginProcessHost* PpapiPluginProcessHost::CreatePluginHost(
    const content::PepperPluginInfo& info,
    const FilePath& profile_data_directory,
    net::HostResolver* host_resolver) {
  PpapiPluginProcessHost* plugin_host = new PpapiPluginProcessHost(
      info, profile_data_directory, host_resolver);
  if (plugin_host->Init(info))
    return plugin_host;

  NOTREACHED();  // Init is not expected to fail.
  return NULL;
}
