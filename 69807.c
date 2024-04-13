NTSTATUS ReadDeviceSkipUnreadableSectors (PDEVICE_OBJECT deviceObject, byte *buffer, LARGE_INTEGER startOffset, ULONG size, uint64 *badSectorCount)
{
	NTSTATUS status;
	ULONG sectorSize;
	ULONG sectorCount;

	*badSectorCount = 0;

	status = GetDeviceSectorSize (deviceObject, &sectorSize);
	if (!NT_SUCCESS (status))
		return status;

	for (sectorCount = size / sectorSize; sectorCount > 0; --sectorCount, startOffset.QuadPart += sectorSize, buffer += sectorSize)
	{
		status = TCReadDevice (deviceObject, buffer, startOffset, sectorSize);
		if (!NT_SUCCESS (status))
		{
			Dump ("Skipping bad sector at %I64d\n", startOffset.QuadPart);
			memset (buffer, 0, sectorSize);
			++(*badSectorCount);
		}
	}

	return STATUS_SUCCESS;
}
