static UINT dvcman_write_channel(IWTSVirtualChannel* pChannel, ULONG cbSize,
                                 const BYTE* pBuffer, void* pReserved)
{
	UINT status;
	DVCMAN_CHANNEL* channel = (DVCMAN_CHANNEL*) pChannel;

	if (!channel || !channel->dvcman)
		return CHANNEL_RC_BAD_CHANNEL;

	EnterCriticalSection(&(channel->lock));
	status = drdynvc_write_data(channel->dvcman->drdynvc,
	                            channel->channel_id, pBuffer, cbSize);
	LeaveCriticalSection(&(channel->lock));
	return status;
}
