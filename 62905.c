static UINT drdynvc_write_data(drdynvcPlugin* drdynvc, UINT32 ChannelId,
                               const BYTE* data, UINT32 dataSize)
{
	wStream* data_out;
	size_t pos;
	UINT32 cbChId;
	UINT32 cbLen;
	unsigned long chunkLength;
	UINT status;

	if (!drdynvc)
		return CHANNEL_RC_BAD_CHANNEL_HANDLE;

	WLog_Print(drdynvc->log, WLOG_DEBUG, "write_data: ChannelId=%"PRIu32" size=%"PRIu32"", ChannelId,
	           dataSize);
	data_out = Stream_New(NULL, CHANNEL_CHUNK_LENGTH);

	if (!data_out)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "Stream_New failed!");
		return CHANNEL_RC_NO_MEMORY;
	}

	Stream_SetPosition(data_out, 1);
	cbChId = drdynvc_write_variable_uint(data_out, ChannelId);
	pos = Stream_GetPosition(data_out);

	if (dataSize == 0)
	{
		Stream_SetPosition(data_out, 0);
		Stream_Write_UINT8(data_out, 0x40 | cbChId);
		Stream_SetPosition(data_out, pos);
		status = drdynvc_send(drdynvc, data_out);
	}
	else if (dataSize <= CHANNEL_CHUNK_LENGTH - pos)
	{
		Stream_SetPosition(data_out, 0);
		Stream_Write_UINT8(data_out, 0x30 | cbChId);
		Stream_SetPosition(data_out, pos);
		Stream_Write(data_out, data, dataSize);
		status = drdynvc_send(drdynvc, data_out);
	}
	else
	{
		/* Fragment the data */
		cbLen = drdynvc_write_variable_uint(data_out, dataSize);
		pos = Stream_GetPosition(data_out);
		Stream_SetPosition(data_out, 0);
		Stream_Write_UINT8(data_out, 0x20 | cbChId | (cbLen << 2));
		Stream_SetPosition(data_out, pos);
		chunkLength = CHANNEL_CHUNK_LENGTH - pos;
		Stream_Write(data_out, data, chunkLength);
		data += chunkLength;
		dataSize -= chunkLength;
		status = drdynvc_send(drdynvc, data_out);

		while (status == CHANNEL_RC_OK && dataSize > 0)
		{
			data_out = Stream_New(NULL, CHANNEL_CHUNK_LENGTH);

			if (!data_out)
			{
				WLog_Print(drdynvc->log, WLOG_ERROR, "Stream_New failed!");
				return CHANNEL_RC_NO_MEMORY;
			}

			Stream_SetPosition(data_out, 1);
			cbChId = drdynvc_write_variable_uint(data_out, ChannelId);
			pos = Stream_GetPosition(data_out);
			Stream_SetPosition(data_out, 0);
			Stream_Write_UINT8(data_out, 0x30 | cbChId);
			Stream_SetPosition(data_out, pos);
			chunkLength = dataSize;

			if (chunkLength > CHANNEL_CHUNK_LENGTH - pos)
				chunkLength = CHANNEL_CHUNK_LENGTH - pos;

			Stream_Write(data_out, data, chunkLength);
			data += chunkLength;
			dataSize -= chunkLength;
			status = drdynvc_send(drdynvc, data_out);
		}
	}

	if (status != CHANNEL_RC_OK)
	{
		WLog_Print(drdynvc->log, WLOG_ERROR, "VirtualChannelWriteEx failed with %s [%08"PRIX32"]",
		           WTSErrorToString(status), status);
		return status;
	}

	return CHANNEL_RC_OK;
}
