NTSTATUS UnmountAllDevices (UNMOUNT_STRUCT *unmountRequest, BOOL ignoreOpenFiles)
{
	NTSTATUS status = 0;
	PDEVICE_OBJECT ListDevice;
	int maxUniqueId = LastUniqueVolumeId;

	Dump ("Unmounting all volumes\n");

	if (unmountRequest)
		unmountRequest->HiddenVolumeProtectionTriggered = FALSE;

	while ((ListDevice = FindVolumeWithHighestUniqueId (maxUniqueId)) != NULL)
	{
		PEXTENSION ListExtension = (PEXTENSION) ListDevice->DeviceExtension;
		maxUniqueId = ListExtension->UniqueVolumeId - 1;

		if (IsVolumeAccessibleByCurrentUser (ListExtension))
		{
			NTSTATUS ntStatus;

			if (unmountRequest)
				unmountRequest->nDosDriveNo = ListExtension->nDosDriveNo;

			ntStatus = UnmountDevice (unmountRequest, ListDevice, ignoreOpenFiles);
			status = ntStatus == 0 ? status : ntStatus;

			if (unmountRequest && unmountRequest->HiddenVolumeProtectionTriggered)
				break;
		}
	}

	return status;
}
