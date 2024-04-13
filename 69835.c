NTSTATUS UnmountDevice (UNMOUNT_STRUCT *unmountRequest, PDEVICE_OBJECT deviceObject, BOOL ignoreOpenFiles)
{
	PEXTENSION extension = deviceObject->DeviceExtension;
	NTSTATUS ntStatus;
	HANDLE volumeHandle;
	PFILE_OBJECT volumeFileObject;

	Dump ("UnmountDevice %d\n", extension->nDosDriveNo);

	ntStatus = TCOpenFsVolume (extension, &volumeHandle, &volumeFileObject);

	if (NT_SUCCESS (ntStatus))
	{
		int dismountRetry;

		if (IsOSAtLeast (WIN_7) && !extension->bReadOnly)
		{
			NTFS_VOLUME_DATA_BUFFER ntfsData;

			if (NT_SUCCESS (TCFsctlCall (volumeFileObject, FSCTL_GET_NTFS_VOLUME_DATA, NULL, 0, &ntfsData, sizeof (ntfsData))))
				DriverUnloadDisabled = TRUE;
		}

		ntStatus = TCFsctlCall (volumeFileObject, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0);
		Dump ("FSCTL_LOCK_VOLUME returned %X\n", ntStatus);

		if (!NT_SUCCESS (ntStatus) && !ignoreOpenFiles)
		{
			TCCloseFsVolume (volumeHandle, volumeFileObject);
			return ERR_FILES_OPEN;
		}

		for (dismountRetry = 0; dismountRetry < 200; ++dismountRetry)
		{
			ntStatus = TCFsctlCall (volumeFileObject, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0);
			Dump ("FSCTL_DISMOUNT_VOLUME returned %X\n", ntStatus);

			if (NT_SUCCESS (ntStatus) || ntStatus == STATUS_VOLUME_DISMOUNTED)
				break;

			if (!ignoreOpenFiles)
			{
				TCCloseFsVolume (volumeHandle, volumeFileObject);
				return ERR_FILES_OPEN;
			}

			TCSleep (100);
		}
	}
	else
	{
		if (!ignoreOpenFiles)
			return ERR_FILES_OPEN;
		else
			volumeHandle = NULL;
	}

	if (extension->bMountManager)
		MountManagerUnmount (extension->nDosDriveNo);

	RemoveDriveLink (extension->nDosDriveNo);

	extension->bShuttingDown = TRUE;

	ntStatus = IoAcquireRemoveLock (&extension->Queue.RemoveLock, NULL);
	ASSERT (NT_SUCCESS (ntStatus));
	IoReleaseRemoveLockAndWait (&extension->Queue.RemoveLock, NULL);

	if (volumeHandle != NULL)
		TCCloseFsVolume (volumeHandle, volumeFileObject);

	if (unmountRequest)
	{
		PCRYPTO_INFO cryptoInfo = ((PEXTENSION) deviceObject->DeviceExtension)->cryptoInfo;
		unmountRequest->HiddenVolumeProtectionTriggered = (cryptoInfo->bProtectHiddenVolume && cryptoInfo->bHiddenVolProtectionAction);
	}

	TCDeleteDeviceObject (deviceObject, (PEXTENSION) deviceObject->DeviceExtension);
	return 0;
}
