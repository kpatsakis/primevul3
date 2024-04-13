 static void zgfx_history_buffer_ring_write(ZGFX_CONTEXT* zgfx, const BYTE* src, size_t count)
 {
	UINT32 front;

	if (count <= 0)
		return;

	if (count > zgfx->HistoryBufferSize)
	{
		const size_t residue = count - zgfx->HistoryBufferSize;
		count = zgfx->HistoryBufferSize;
		src += residue;
		zgfx->HistoryIndex = (zgfx->HistoryIndex + residue) % zgfx->HistoryBufferSize;
	}

	if (zgfx->HistoryIndex + count <= zgfx->HistoryBufferSize)
	{
		CopyMemory(&(zgfx->HistoryBuffer[zgfx->HistoryIndex]), src, count);

		if ((zgfx->HistoryIndex += count) == zgfx->HistoryBufferSize)
			zgfx->HistoryIndex = 0;
	}
	else
	{
		front = zgfx->HistoryBufferSize - zgfx->HistoryIndex;
		CopyMemory(&(zgfx->HistoryBuffer[zgfx->HistoryIndex]), src, front);
		CopyMemory(zgfx->HistoryBuffer, &src[front], count - front);
		zgfx->HistoryIndex = count - front;
	}
}
