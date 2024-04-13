static UINT dvcman_close_channel_iface(IWTSVirtualChannel* pChannel)
{
	DVCMAN_CHANNEL* channel = (DVCMAN_CHANNEL*) pChannel;

	if (!channel)
		return CHANNEL_RC_BAD_CHANNEL;

	WLog_DBG(TAG, "close_channel_iface: id=%"PRIu32"", channel->channel_id);
	return CHANNEL_RC_OK;
}
