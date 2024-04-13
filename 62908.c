static DVCMAN_CHANNEL* dvcman_channel_new(drdynvcPlugin* drdynvc,
        IWTSVirtualChannelManager* pChannelMgr,
        UINT32 ChannelId, const char* ChannelName)
{
	DVCMAN_CHANNEL* channel;

	if (dvcman_find_channel_by_id(pChannelMgr, ChannelId))
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "Protocol error: Duplicated ChannelId %"PRIu32" (%s)!",
		           ChannelId,
		           ChannelName);
		return NULL;
	}

	channel = (DVCMAN_CHANNEL*) calloc(1, sizeof(DVCMAN_CHANNEL));

	if (!channel)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "calloc failed!");
		return NULL;
	}

	channel->dvcman = (DVCMAN*) pChannelMgr;
	channel->channel_id = ChannelId;
	channel->channel_name = _strdup(ChannelName);

	if (!channel->channel_name)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "_strdup failed!");
		free(channel);
		return NULL;
	}

	if (!InitializeCriticalSectionEx(&(channel->lock), 0, 0))
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "InitializeCriticalSectionEx failed!");
		free(channel->channel_name);
		free(channel);
		return NULL;
	}

	return channel;
}
