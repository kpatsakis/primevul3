NSC_MESSAGE* nsc_encode_messages(NSC_CONTEXT* context, const BYTE* data,
                                 UINT32 x, UINT32 y, UINT32 width, UINT32 height,
                                 UINT32 scanline, UINT32* numMessages,
                                 UINT32 maxDataSize)
{
	UINT32 i, j, k;
	UINT32 dataOffset;
	UINT32 rows, cols;
	UINT32 BytesPerPixel;
	UINT32 MaxRegionWidth;
	UINT32 MaxRegionHeight;
	UINT32 ByteCount[4];
	UINT32 MaxPlaneSize;
	UINT32 MaxMessageSize;
	NSC_MESSAGE* messages;
	UINT32 PaddedMaxPlaneSize;
	k = 0;
	MaxRegionWidth = 64 * 4;
	MaxRegionHeight = 64 * 2;
	BytesPerPixel = GetBytesPerPixel(context->format);
	rows = (width + (MaxRegionWidth - (width % MaxRegionWidth))) / MaxRegionWidth;
	cols = (height + (MaxRegionHeight - (height % MaxRegionHeight))) /
	       MaxRegionHeight;
	*numMessages = rows * cols;
	MaxPlaneSize = nsc_compute_byte_count(context, (UINT32*) ByteCount, width,
	                                      height);
	MaxMessageSize = ByteCount[0] + ByteCount[1] + ByteCount[2] + ByteCount[3] + 20;
	maxDataSize -= 1024; /* reserve enough space for headers */
	messages = (NSC_MESSAGE*) calloc(*numMessages, sizeof(NSC_MESSAGE));

	if (!messages)
		return NULL;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			messages[k].x = x + (i * MaxRegionWidth);
			messages[k].y = y + (j * MaxRegionHeight);
			messages[k].width = (i < (rows - 1)) ? MaxRegionWidth : width -
			                    (i * MaxRegionWidth);
			messages[k].height = (j < (cols - 1)) ? MaxRegionHeight : height -
			                     (j * MaxRegionHeight);
			messages[k].data = data;
			messages[k].scanline = scanline;
			messages[k].MaxPlaneSize = nsc_compute_byte_count(context,
			                           (UINT32*) messages[k].OrgByteCount, messages[k].width, messages[k].height);
			k++;
		}
	}

	*numMessages = k;

	for (i = 0; i < *numMessages; i++)
	{
		PaddedMaxPlaneSize = messages[i].MaxPlaneSize + 32;
		messages[i].PlaneBuffer = (BYTE*) BufferPool_Take(context->priv->PlanePool,
		                          PaddedMaxPlaneSize * 5);

		if (!messages[i].PlaneBuffer)
			goto fail;

		messages[i].PlaneBuffers[0] = (BYTE*) &
		                              (messages[i].PlaneBuffer[(PaddedMaxPlaneSize * 0) + 16]);
		messages[i].PlaneBuffers[1] = (BYTE*) &
		                              (messages[i].PlaneBuffer[(PaddedMaxPlaneSize * 1) + 16]);
		messages[i].PlaneBuffers[2] = (BYTE*) &
		                              (messages[i].PlaneBuffer[(PaddedMaxPlaneSize * 2) + 16]);
		messages[i].PlaneBuffers[3] = (BYTE*) &
		                              (messages[i].PlaneBuffer[(PaddedMaxPlaneSize * 3) + 16]);
		messages[i].PlaneBuffers[4] = (BYTE*) &
		                              (messages[i].PlaneBuffer[(PaddedMaxPlaneSize * 4) + 16]);
	}

	for (i = 0; i < *numMessages; i++)
	{
		context->width = messages[i].width;
		context->height = messages[i].height;
		context->OrgByteCount[0] = messages[i].OrgByteCount[0];
		context->OrgByteCount[1] = messages[i].OrgByteCount[1];
		context->OrgByteCount[2] = messages[i].OrgByteCount[2];
		context->OrgByteCount[3] = messages[i].OrgByteCount[3];
		context->priv->PlaneBuffersLength = messages[i].MaxPlaneSize;
		context->priv->PlaneBuffers[0] = messages[i].PlaneBuffers[0];
		context->priv->PlaneBuffers[1] = messages[i].PlaneBuffers[1];
		context->priv->PlaneBuffers[2] = messages[i].PlaneBuffers[2];
		context->priv->PlaneBuffers[3] = messages[i].PlaneBuffers[3];
		context->priv->PlaneBuffers[4] = messages[i].PlaneBuffers[4];
		dataOffset = (messages[i].y * messages[i].scanline) + (messages[i].x *
		             BytesPerPixel);
		PROFILER_ENTER(context->priv->prof_nsc_encode)
		context->encode(context, &data[dataOffset], scanline);
		PROFILER_EXIT(context->priv->prof_nsc_encode)
		PROFILER_ENTER(context->priv->prof_nsc_rle_compress_data)
		nsc_rle_compress_data(context);
		PROFILER_EXIT(context->priv->prof_nsc_rle_compress_data)
		messages[i].LumaPlaneByteCount = context->PlaneByteCount[0];
		messages[i].OrangeChromaPlaneByteCount = context->PlaneByteCount[1];
		messages[i].GreenChromaPlaneByteCount = context->PlaneByteCount[2];
		messages[i].AlphaPlaneByteCount = context->PlaneByteCount[3];
		messages[i].ColorLossLevel = context->ColorLossLevel;
		messages[i].ChromaSubsamplingLevel = context->ChromaSubsamplingLevel;
	}

	context->priv->PlaneBuffers[0] = NULL;
	context->priv->PlaneBuffers[1] = NULL;
	context->priv->PlaneBuffers[2] = NULL;
	context->priv->PlaneBuffers[3] = NULL;
	context->priv->PlaneBuffers[4] = NULL;
	return messages;
fail:

	for (i = 0; i < *numMessages; i++)
		BufferPool_Return(context->priv->PlanePool, messages[i].PlaneBuffer);

	free(messages);
	return NULL;
}
