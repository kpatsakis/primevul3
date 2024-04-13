static IWTSVirtualChannelManager* dvcman_new(drdynvcPlugin* plugin)
{
	DVCMAN* dvcman;
	dvcman = (DVCMAN*) calloc(1, sizeof(DVCMAN));

	if (!dvcman)
	{
		WLog_Print(plugin->log, WLOG_ERROR, "calloc failed!");
		return NULL;
	}

	dvcman->iface.CreateListener = dvcman_create_listener;
	dvcman->iface.FindChannelById = dvcman_find_channel_by_id;
	dvcman->iface.GetChannelId = dvcman_get_channel_id;
	dvcman->drdynvc = plugin;
	dvcman->channels = ArrayList_New(TRUE);

	if (!dvcman->channels)
	{
		WLog_Print(plugin->log, WLOG_ERROR, "ArrayList_New failed!");
		free(dvcman);
		return NULL;
	}

	dvcman->channels->object.fnObjectFree = dvcman_channel_free;
	dvcman->pool = StreamPool_New(TRUE, 10);

	if (!dvcman->pool)
	{
		WLog_Print(plugin->log, WLOG_ERROR, "StreamPool_New failed!");
		ArrayList_Free(dvcman->channels);
		free(dvcman);
		return NULL;
	}

	return (IWTSVirtualChannelManager*) dvcman;
}
