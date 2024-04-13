NTSTATUS MountManagerMount (MOUNT_STRUCT *mount)
{
	NTSTATUS ntStatus;
	WCHAR arrVolume[256];
	char buf[200];
	PMOUNTMGR_TARGET_NAME in = (PMOUNTMGR_TARGET_NAME) buf;
	PMOUNTMGR_CREATE_POINT_INPUT point = (PMOUNTMGR_CREATE_POINT_INPUT) buf;

	TCGetNTNameFromNumber (arrVolume, sizeof(arrVolume),mount->nDosDriveNo);
	in->DeviceNameLength = (USHORT) wcslen (arrVolume) * 2;
	RtlStringCbCopyW(in->DeviceName, sizeof(buf) - sizeof(in->DeviceNameLength),arrVolume);

	ntStatus = TCDeviceIoControl (MOUNTMGR_DEVICE_NAME, IOCTL_MOUNTMGR_VOLUME_ARRIVAL_NOTIFICATION,
		in, (ULONG) (sizeof (in->DeviceNameLength) + wcslen (arrVolume) * 2), 0, 0);

	memset (buf, 0, sizeof buf);
	TCGetDosNameFromNumber ((PWSTR) &point[1], sizeof(buf) - sizeof(MOUNTMGR_CREATE_POINT_INPUT),mount->nDosDriveNo, DeviceNamespaceDefault);

	point->SymbolicLinkNameOffset = sizeof (MOUNTMGR_CREATE_POINT_INPUT);
	point->SymbolicLinkNameLength = (USHORT) wcslen ((PWSTR) &point[1]) * 2;

	point->DeviceNameOffset = point->SymbolicLinkNameOffset + point->SymbolicLinkNameLength;
	TCGetNTNameFromNumber ((PWSTR) (buf + point->DeviceNameOffset), sizeof(buf) - point->DeviceNameOffset,mount->nDosDriveNo);
	point->DeviceNameLength = (USHORT) wcslen ((PWSTR) (buf + point->DeviceNameOffset)) * 2;

	ntStatus = TCDeviceIoControl (MOUNTMGR_DEVICE_NAME, IOCTL_MOUNTMGR_CREATE_POINT, point,
			point->DeviceNameOffset + point->DeviceNameLength, 0, 0);

	return ntStatus;
}
