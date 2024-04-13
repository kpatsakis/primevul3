static void zgfx_history_buffer_ring_read(ZGFX_CONTEXT* zgfx, int offset, BYTE* dst, UINT32 count)
{
	UINT32 front;
	UINT32 index;
	UINT32 bytes;
	UINT32 valid;
	UINT32 bytesLeft;
	BYTE* dptr = dst;
	BYTE* origDst = dst;

	if (count <= 0)
		return;

	bytesLeft = count;
	index = (zgfx->HistoryIndex + zgfx->HistoryBufferSize - offset) % zgfx->HistoryBufferSize;
	bytes = MIN(bytesLeft, offset);

	if ((index + bytes) <= zgfx->HistoryBufferSize)
	{
		CopyMemory(dptr, &(zgfx->HistoryBuffer[index]), bytes);
	}
	else
	{
		front = zgfx->HistoryBufferSize - index;
		CopyMemory(dptr, &(zgfx->HistoryBuffer[index]), front);
		CopyMemory(&dptr[front], zgfx->HistoryBuffer, bytes - front);
	}

	if ((bytesLeft -= bytes) == 0)
		return;

	dptr += bytes;
	valid = bytes;

	do
	{
		bytes = valid;

		if (bytes > bytesLeft)
			bytes = bytesLeft;

		CopyMemory(dptr, origDst, bytes);
		dptr += bytes;
		valid <<= 1;
	}
	while ((bytesLeft -= bytes) > 0);
}
