static UINT dvcman_load_addin(drdynvcPlugin* drdynvc,
                              IWTSVirtualChannelManager* pChannelMgr,
                              ADDIN_ARGV* args,
                              rdpSettings* settings)
{
	DVCMAN_ENTRY_POINTS entryPoints;
	PDVC_PLUGIN_ENTRY pDVCPluginEntry = NULL;
	WLog_Print(drdynvc->log, WLOG_INFO, "Loading Dynamic Virtual Channel %s", args->argv[0]);
	pDVCPluginEntry = (PDVC_PLUGIN_ENTRY) freerdp_load_channel_addin_entry(
	                      args->argv[0],
	                      NULL, NULL, FREERDP_ADDIN_CHANNEL_DYNAMIC);

	if (pDVCPluginEntry)
	{
		entryPoints.iface.RegisterPlugin = dvcman_register_plugin;
		entryPoints.iface.GetPlugin = dvcman_get_plugin;
		entryPoints.iface.GetPluginData = dvcman_get_plugin_data;
		entryPoints.iface.GetRdpSettings = dvcman_get_rdp_settings;
		entryPoints.dvcman = (DVCMAN*) pChannelMgr;
		entryPoints.args = args;
		entryPoints.settings = settings;
		return pDVCPluginEntry((IDRDYNVC_ENTRY_POINTS*) &entryPoints);
	}

	return ERROR_INVALID_FUNCTION;
}
