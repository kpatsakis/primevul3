UINT32 nsc_compute_byte_count(NSC_CONTEXT* context, UINT32* ByteCount,
                              UINT32 width, UINT32 height)
{
	UINT32 tempWidth;
	UINT32 tempHeight;
	UINT32 maxPlaneSize;
	tempWidth = ROUND_UP_TO(width, 8);
	tempHeight = ROUND_UP_TO(height, 2);
	maxPlaneSize = tempWidth * tempHeight + 16;

	if (context->ChromaSubsamplingLevel)
	{
		ByteCount[0] = tempWidth * height;
		ByteCount[1] = tempWidth * tempHeight / 4;
		ByteCount[2] = tempWidth * tempHeight / 4;
		ByteCount[3] = width * height;
	}
	else
	{
		ByteCount[0] = width * height;
		ByteCount[1] = width * height;
		ByteCount[2] = width * height;
		ByteCount[3] = width * height;
	}

	return maxPlaneSize;
}
