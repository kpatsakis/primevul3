static BOOL nsc_context_initialize(NSC_CONTEXT* context, wStream* s)
{
	int i;
	UINT32 length;
	UINT32 tempWidth;
	UINT32 tempHeight;

	if (!nsc_stream_initialize(context, s))
		return FALSE;

	length = context->width * context->height * 4;

	if (!context->BitmapData)
	{
		context->BitmapData = calloc(1, length + 16);

		if (!context->BitmapData)
			return FALSE;

		context->BitmapDataLength = length;
	}
	else if (length > context->BitmapDataLength)
	{
		void* tmp;
		tmp = realloc(context->BitmapData, length + 16);

		if (!tmp)
			return FALSE;

		context->BitmapData = tmp;
		context->BitmapDataLength = length;
	}

	tempWidth = ROUND_UP_TO(context->width, 8);
	tempHeight = ROUND_UP_TO(context->height, 2);
	/* The maximum length a decoded plane can reach in all cases */
	length = tempWidth * tempHeight;

	if (length > context->priv->PlaneBuffersLength)
	{
		for (i = 0; i < 4; i++)
		{
			void* tmp = (BYTE*) realloc(context->priv->PlaneBuffers[i], length);

			if (!tmp)
				return FALSE;

			context->priv->PlaneBuffers[i] = tmp;
		}

		context->priv->PlaneBuffersLength = length;
	}

	for (i = 0; i < 4; i++)
	{
		context->OrgByteCount[i] = context->width * context->height;
	}

	if (context->ChromaSubsamplingLevel)
	{
		context->OrgByteCount[0] = tempWidth * context->height;
		context->OrgByteCount[1] = (tempWidth >> 1) * (tempHeight >> 1);
		context->OrgByteCount[2] = context->OrgByteCount[1];
	}

	return TRUE;
}
