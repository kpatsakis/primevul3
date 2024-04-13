static ADDIN_ARGV* dvcman_get_plugin_data(IDRDYNVC_ENTRY_POINTS* pEntryPoints)
{
	return ((DVCMAN_ENTRY_POINTS*) pEntryPoints)->args;
}
