NTSTATUS ZeroUnreadableSectors (PDEVICE_OBJECT deviceObject, LARGE_INTEGER startOffset, ULONG size, uint64 *zeroedSectorCount)
{
	NTSTATUS status;
	ULONG sectorSize;
	ULONG sectorCount;
	byte *sectorBuffer = NULL;

	*zeroedSectorCount = 0;

	status = GetDeviceSectorSize (deviceObject, &sectorSize);
	if (!NT_SUCCESS (status))
		return status;

	sectorBuffer = TCalloc (sectorSize);
	if (!sectorBuffer)
		return STATUS_INSUFFICIENT_RESOURCES;

	for (sectorCount = size / sectorSize; sectorCount > 0; --sectorCount, startOffset.QuadPart += sectorSize)
	{
		status = TCReadDevice (deviceObject, sectorBuffer, startOffset, sectorSize);
		if (!NT_SUCCESS (status))
		{
			Dump ("Zeroing sector at %I64d\n", startOffset.QuadPart);
			memset (sectorBuffer, 0, sectorSize);

			status = TCWriteDevice (deviceObject, sectorBuffer, startOffset, sectorSize);
			if (!NT_SUCCESS (status))
				goto err;

			++(*zeroedSectorCount);
		}
	}

	status = STATUS_SUCCESS;

err:
	if (sectorBuffer)
		TCfree (sectorBuffer);

	return status;
}
