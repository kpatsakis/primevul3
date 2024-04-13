static void* dvcman_get_rdp_settings(IDRDYNVC_ENTRY_POINTS* pEntryPoints)
{
	return (void*)((DVCMAN_ENTRY_POINTS*) pEntryPoints)->settings;
}
