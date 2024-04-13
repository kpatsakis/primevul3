static UINT drdynvc_virtual_channel_event_terminated(drdynvcPlugin* drdynvc)
{
	if (!drdynvc)
		return CHANNEL_RC_BAD_CHANNEL_HANDLE;

	drdynvc->InitHandle = 0;
	free(drdynvc->context);
	free(drdynvc);
	return CHANNEL_RC_OK;
}
