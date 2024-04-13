static UINT32 dvcman_get_channel_id(IWTSVirtualChannel* channel)
{
	return ((DVCMAN_CHANNEL*) channel)->channel_id;
}
