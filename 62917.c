static IWTSPlugin* dvcman_get_plugin(IDRDYNVC_ENTRY_POINTS* pEntryPoints,
                                     const char* name)
{
	int i;
	DVCMAN* dvcman = ((DVCMAN_ENTRY_POINTS*) pEntryPoints)->dvcman;

	for (i = 0; i < dvcman->num_plugins; i++)
	{
		if (dvcman->plugin_names[i] == name ||
		    strcmp(dvcman->plugin_names[i], name) == 0)
		{
			return dvcman->plugins[i];
		}
	}

	return NULL;
}
