NTSTATUS ProbeRealDriveSize (PDEVICE_OBJECT driveDeviceObject, LARGE_INTEGER *driveSize)
{
	NTSTATUS status;
	LARGE_INTEGER sysLength;
	LARGE_INTEGER offset;
	byte *sectorBuffer;
	ULONGLONG startTime;

	if (!UserCanAccessDriveDevice())
		return STATUS_ACCESS_DENIED;

	sectorBuffer = TCalloc (TC_SECTOR_SIZE_BIOS);
	if (!sectorBuffer)
		return STATUS_INSUFFICIENT_RESOURCES;

	status = SendDeviceIoControlRequest (driveDeviceObject, IOCTL_DISK_GET_LENGTH_INFO,
		NULL, 0, &sysLength, sizeof (sysLength));

	if (!NT_SUCCESS (status))
	{
		Dump ("Failed to get drive size - error %x\n", status);
		TCfree (sectorBuffer);
		return status;
	}

	startTime = KeQueryInterruptTime ();
	for (offset.QuadPart = sysLength.QuadPart; ; offset.QuadPart += TC_SECTOR_SIZE_BIOS)
	{
		status = TCReadDevice (driveDeviceObject, sectorBuffer, offset, TC_SECTOR_SIZE_BIOS);

		if (NT_SUCCESS (status))
			status = TCWriteDevice (driveDeviceObject, sectorBuffer, offset, TC_SECTOR_SIZE_BIOS);

		if (!NT_SUCCESS (status))
		{
			driveSize->QuadPart = offset.QuadPart;
			Dump ("Real drive size = %I64d bytes (%I64d hidden)\n", driveSize->QuadPart, driveSize->QuadPart - sysLength.QuadPart);
			TCfree (sectorBuffer);
			return STATUS_SUCCESS;
		}

		if (KeQueryInterruptTime() - startTime > 3ULL * 60 * 1000 * 1000 * 10)
		{
			driveSize->QuadPart = sysLength.QuadPart;
			TCfree (sectorBuffer);
			return STATUS_TIMEOUT;
		}
	}
}
