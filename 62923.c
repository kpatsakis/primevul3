static UINT dvcman_receive_channel_data_first(drdynvcPlugin* drdynvc,
        IWTSVirtualChannelManager* pChannelMgr,
        UINT32 ChannelId, UINT32 length)
{
	DVCMAN_CHANNEL* channel;
	channel = (DVCMAN_CHANNEL*) dvcman_find_channel_by_id(pChannelMgr, ChannelId);

	if (!channel)
	{
		/**
		 * Windows Server 2012 R2 can send some messages over Microsoft::Windows::RDS::Geometry::v08.01
		 * even if the dynamic virtual channel wasn't registered on our side. Ignoring it works.
		 */
		WLog_Print(drdynvc->log, WLOG_ERROR, "ChannelId %"PRIu32" not found!", ChannelId);
		return CHANNEL_RC_OK;
	}

	if (channel->dvc_data)
		Stream_Release(channel->dvc_data);

	channel->dvc_data = StreamPool_Take(channel->dvcman->pool, length);

	if (!channel->dvc_data)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "StreamPool_Take failed!");
		return CHANNEL_RC_NO_MEMORY;
	}

	channel->dvc_data_length = length;
	return CHANNEL_RC_OK;
}
