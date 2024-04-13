static IWTSVirtualChannel* dvcman_find_channel_by_id(IWTSVirtualChannelManager*
        pChannelMgr,
        UINT32 ChannelId)
{
	int index;
	BOOL found = FALSE;
	DVCMAN_CHANNEL* channel;
	DVCMAN* dvcman = (DVCMAN*) pChannelMgr;
	ArrayList_Lock(dvcman->channels);
	index = 0;
	channel = (DVCMAN_CHANNEL*) ArrayList_GetItem(dvcman->channels, index++);

	while (channel)
	{
		if (channel->channel_id == ChannelId)
		{
			found = TRUE;
			break;
		}

		channel = (DVCMAN_CHANNEL*) ArrayList_GetItem(dvcman->channels, index++);
	}

	ArrayList_Unlock(dvcman->channels);
	return (found) ? ((IWTSVirtualChannel*) channel) : NULL;
}
