void *AllocateMemoryWithTimeout (size_t size, int retryDelay, int timeout)
{
	LARGE_INTEGER waitInterval;
	waitInterval.QuadPart = retryDelay * -10000;

	ASSERT (KeGetCurrentIrql() <= APC_LEVEL);
	ASSERT (retryDelay > 0 && retryDelay <= timeout);

	while (TRUE)
	{
		void *memory = TCalloc (size);
		if (memory)
			return memory;

		timeout -= retryDelay;
		if (timeout <= 0)
			break;

		KeDelayExecutionThread (KernelMode, FALSE, &waitInterval);
	}

	return NULL;
}
