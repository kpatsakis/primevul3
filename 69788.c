NTSTATUS GetDeviceSectorSize (PDEVICE_OBJECT deviceObject, ULONG *bytesPerSector)
{
	NTSTATUS status;
	DISK_GEOMETRY geometry;

	status = SendDeviceIoControlRequest (deviceObject, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &geometry, sizeof (geometry));
	if (!NT_SUCCESS (status))
		return status;

	*bytesPerSector = geometry.BytesPerSector;
	
	return STATUS_SUCCESS;
}
