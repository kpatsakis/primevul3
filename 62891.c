BOOL VCAPITYPE VirtualChannelEntryEx(PCHANNEL_ENTRY_POINTS_EX pEntryPoints, PVOID pInitHandle)
{
	UINT rc;
	drdynvcPlugin* drdynvc;
	DrdynvcClientContext* context = NULL;
	CHANNEL_ENTRY_POINTS_FREERDP_EX* pEntryPointsEx;
	drdynvc = (drdynvcPlugin*) calloc(1, sizeof(drdynvcPlugin));

	if (!drdynvc)
	{
		WLog_ERR(TAG, "calloc failed!");
		return FALSE;
	}

	drdynvc->channelDef.options =
	    CHANNEL_OPTION_INITIALIZED |
	    CHANNEL_OPTION_ENCRYPT_RDP |
	    CHANNEL_OPTION_COMPRESS_RDP;
	sprintf_s(drdynvc->channelDef.name, ARRAYSIZE(drdynvc->channelDef.name), "drdynvc");
	drdynvc->state = DRDYNVC_STATE_INITIAL;
	pEntryPointsEx = (CHANNEL_ENTRY_POINTS_FREERDP_EX*) pEntryPoints;

	if ((pEntryPointsEx->cbSize >= sizeof(CHANNEL_ENTRY_POINTS_FREERDP_EX)) &&
	    (pEntryPointsEx->MagicNumber == FREERDP_CHANNEL_MAGIC_NUMBER))
	{
		context = (DrdynvcClientContext*) calloc(1, sizeof(DrdynvcClientContext));

		if (!context)
		{
			WLog_Print(drdynvc->log, WLOG_ERROR, "calloc failed!");
			free(drdynvc);
			return FALSE;
		}

		context->handle = (void*) drdynvc;
		context->custom = NULL;
		drdynvc->context = context;
		context->GetVersion = drdynvc_get_version;
		drdynvc->rdpcontext = pEntryPointsEx->context;
	}

	drdynvc->log = WLog_Get(TAG);
	WLog_Print(drdynvc->log, WLOG_DEBUG, "VirtualChannelEntryEx");
	CopyMemory(&(drdynvc->channelEntryPoints), pEntryPoints, sizeof(CHANNEL_ENTRY_POINTS_FREERDP_EX));
	drdynvc->InitHandle = pInitHandle;
	rc = drdynvc->channelEntryPoints.pVirtualChannelInitEx(drdynvc, context, pInitHandle,
	        &drdynvc->channelDef, 1, VIRTUAL_CHANNEL_VERSION_WIN2000, drdynvc_virtual_channel_init_event_ex);

	if (CHANNEL_RC_OK != rc)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "pVirtualChannelInit failed with %s [%08"PRIX32"]",
		           WTSErrorToString(rc), rc);
		free(drdynvc->context);
		free(drdynvc);
		return FALSE;
	}

	drdynvc->channelEntryPoints.pInterface = context;
	return TRUE;
}
