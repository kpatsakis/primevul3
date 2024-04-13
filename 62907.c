static void dvcman_channel_free(void* arg)
{
	DVCMAN_CHANNEL* channel = (DVCMAN_CHANNEL*) arg;
	UINT error = CHANNEL_RC_OK;

	if (channel)
	{
		if (channel->channel_callback)
		{
			IFCALL(channel->channel_callback->OnClose,
			       channel->channel_callback);
		}

		if (channel->status == CHANNEL_RC_OK)
		{
			IWTSVirtualChannel* ichannel = (IWTSVirtualChannel*) channel;

			if (channel->dvcman && channel->dvcman->drdynvc)
			{
				DrdynvcClientContext* context = channel->dvcman->drdynvc->context;

				if (context)
				{
					IFCALLRET(context->OnChannelDisconnected, error,
					          context, channel->channel_name,
					          channel->pInterface);
				}
			}

			error = IFCALLRESULT(CHANNEL_RC_OK, ichannel->Close, ichannel);

			if (error != CHANNEL_RC_OK)
				WLog_ERR(TAG, "Close failed with error %"PRIu32"!", error);
		}

		if (channel->dvc_data)
			Stream_Release(channel->dvc_data);

		DeleteCriticalSection(&(channel->lock));
		free(channel->channel_name);
	}

	free(channel);
}
