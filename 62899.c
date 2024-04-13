static UINT drdynvc_virtual_channel_event_connected(drdynvcPlugin* drdynvc, LPVOID pData,
        UINT32 dataLength)
{
	UINT error;
	UINT32 status;
	UINT32 index;
	ADDIN_ARGV* args;
	rdpSettings* settings;

	if (!drdynvc)
		return CHANNEL_RC_BAD_CHANNEL_HANDLE;

	status = drdynvc->channelEntryPoints.pVirtualChannelOpenEx(drdynvc->InitHandle,
	         &drdynvc->OpenHandle, drdynvc->channelDef.name, drdynvc_virtual_channel_open_event_ex);

	if (status != CHANNEL_RC_OK)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "pVirtualChannelOpen failed with %s [%08"PRIX32"]",
		           WTSErrorToString(status), status);
		return status;
	}

	drdynvc->queue = MessageQueue_New(NULL);

	if (!drdynvc->queue)
	{
		error = CHANNEL_RC_NO_MEMORY;
		WLog_Print(drdynvc->log, WLOG_ERROR, "MessageQueue_New failed!");
		goto error;
	}

	drdynvc->queue->object.fnObjectFree = drdynvc_queue_object_free;
	drdynvc->channel_mgr = dvcman_new(drdynvc);

	if (!drdynvc->channel_mgr)
	{
		error = CHANNEL_RC_NO_MEMORY;
		WLog_Print(drdynvc->log, WLOG_ERROR, "dvcman_new failed!");
		goto error;
	}

	settings = (rdpSettings*) drdynvc->channelEntryPoints.pExtendedData;

	for (index = 0; index < settings->DynamicChannelCount; index++)
	{
		args = settings->DynamicChannelArray[index];
		error = dvcman_load_addin(drdynvc, drdynvc->channel_mgr, args, settings);

		if (CHANNEL_RC_OK != error)
			goto error;
	}

	if ((error = dvcman_init(drdynvc, drdynvc->channel_mgr)))
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "dvcman_init failed with error %"PRIu32"!", error);
		goto error;
	}

	drdynvc->state = DRDYNVC_STATE_CAPABILITIES;

	if (!(drdynvc->thread = CreateThread(NULL, 0, drdynvc_virtual_channel_client_thread,
	                                     (void*) drdynvc,
	                                     0, NULL)))
	{
		error = ERROR_INTERNAL_ERROR;
		WLog_Print(drdynvc->log, WLOG_ERROR, "CreateThread failed!");
		goto error;
	}

error:
	return error;
}
