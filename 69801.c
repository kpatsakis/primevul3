NTSTATUS MountDevice (PDEVICE_OBJECT DeviceObject, MOUNT_STRUCT *mount)
{
	PDEVICE_OBJECT NewDeviceObject;
	NTSTATUS ntStatus;

	if (mount->nDosDriveNo >= 0 && mount->nDosDriveNo <= 25
		&& IsDriveLetterAvailable (mount->nDosDriveNo, DeviceNamespaceDefault) // drive letter must not exist both locally and globally
		&& IsDriveLetterAvailable (mount->nDosDriveNo, DeviceNamespaceGlobal)
		)
	{
		Dump ("Mount request looks valid\n");
	}
	else
	{
		Dump ("WARNING: MOUNT DRIVE LETTER INVALID\n");
		mount->nReturnCode = ERR_DRIVE_NOT_FOUND;
		return ERR_DRIVE_NOT_FOUND;
	}

	if (!SelfTestsPassed)
	{
		Dump ("Failure of built-in automatic self-tests! Mounting not allowed.\n");
		mount->nReturnCode = ERR_SELF_TESTS_FAILED;
		return ERR_SELF_TESTS_FAILED;
	}

	ntStatus = TCCreateDeviceObject (DeviceObject->DriverObject, &NewDeviceObject, mount);

	if (!NT_SUCCESS (ntStatus))
	{
		Dump ("Mount CREATE DEVICE ERROR, ntStatus = 0x%08x\n", ntStatus);
		return ntStatus;
	}
	else
	{
		PEXTENSION NewExtension = (PEXTENSION) NewDeviceObject->DeviceExtension;
		SECURITY_SUBJECT_CONTEXT subContext;
		PACCESS_TOKEN accessToken;

		SeCaptureSubjectContext (&subContext);
		SeLockSubjectContext(&subContext);
		if (subContext.ClientToken && subContext.ImpersonationLevel >= SecurityImpersonation)
			accessToken = subContext.ClientToken;
		else
			accessToken = subContext.PrimaryToken;

		if (!accessToken)
		{
			ntStatus = STATUS_INVALID_PARAMETER;
		}
		else
		{
			PTOKEN_USER tokenUser;

			ntStatus = SeQueryInformationToken (accessToken, TokenUser, &tokenUser);
			if (NT_SUCCESS (ntStatus))
			{
				ULONG sidLength = RtlLengthSid (tokenUser->User.Sid);

				NewExtension->UserSid = TCalloc (sidLength);
				if (!NewExtension->UserSid)
					ntStatus = STATUS_INSUFFICIENT_RESOURCES;
				else
					ntStatus = RtlCopySid (sidLength, NewExtension->UserSid, tokenUser->User.Sid);

				ExFreePool (tokenUser);		// Documented in newer versions of WDK
			}
		}

		SeUnlockSubjectContext(&subContext);
		SeReleaseSubjectContext (&subContext);

		if (NT_SUCCESS (ntStatus))
			ntStatus = TCStartVolumeThread (NewDeviceObject, NewExtension, mount);

		if (!NT_SUCCESS (ntStatus))
		{
			Dump ("Mount FAILURE NT ERROR, ntStatus = 0x%08x\n", ntStatus);
			TCDeleteDeviceObject (NewDeviceObject, NewExtension);
			return ntStatus;
		}
		else
		{
			if (mount->nReturnCode == 0)
			{
				HANDLE volumeHandle;
				PFILE_OBJECT volumeFileObject;
				ULONG labelLen = (ULONG) wcslen (mount->wszLabel);
				BOOL bIsNTFS = FALSE;
				ULONG labelMaxLen, labelEffectiveLen;

				Dump ("Mount SUCCESS TC code = 0x%08x READ-ONLY = %d\n", mount->nReturnCode, NewExtension->bReadOnly);

				if (NewExtension->bReadOnly)
					NewDeviceObject->Characteristics |= FILE_READ_ONLY_DEVICE;

				NewDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

				NewExtension->UniqueVolumeId = LastUniqueVolumeId++;

				if (	!IsDriveLetterAvailable (mount->nDosDriveNo, DeviceNamespaceDefault)
					|| !IsDriveLetterAvailable (mount->nDosDriveNo, DeviceNamespaceGlobal)
					)
				{
						TCDeleteDeviceObject (NewDeviceObject, NewExtension);
						mount->nReturnCode = ERR_DRIVE_NOT_FOUND;
						return ERR_DRIVE_NOT_FOUND;
				}

				if (mount->bMountManager)
					MountManagerMount (mount);

				NewExtension->bMountManager = mount->bMountManager;

				CreateDriveLink (mount->nDosDriveNo);

				mount->FilesystemDirty = FALSE;

				if (NT_SUCCESS (TCOpenFsVolume (NewExtension, &volumeHandle, &volumeFileObject)))
				{
					__try
					{
						ULONG fsStatus;

						if (NT_SUCCESS (TCFsctlCall (volumeFileObject, FSCTL_IS_VOLUME_DIRTY, NULL, 0, &fsStatus, sizeof (fsStatus)))
							&& (fsStatus & VOLUME_IS_DIRTY))
						{
							mount->FilesystemDirty = TRUE;
						}
					}
					__except (EXCEPTION_EXECUTE_HANDLER)
					{
						mount->FilesystemDirty = TRUE;
					}

					__try
					{
						NTFS_VOLUME_DATA_BUFFER ntfsData;
						if (NT_SUCCESS (TCFsctlCall (volumeFileObject, FSCTL_GET_NTFS_VOLUME_DATA, NULL, 0, &ntfsData, sizeof (ntfsData))))
						{
							bIsNTFS = TRUE;
						}
					}
					__except (EXCEPTION_EXECUTE_HANDLER)
					{
						bIsNTFS = FALSE;
					}

					NewExtension->bIsNTFS = bIsNTFS;
					mount->bIsNTFS = bIsNTFS;

					if (labelLen > 0)
					{
						if (bIsNTFS)
							labelMaxLen = 32; // NTFS maximum label length
						else
							labelMaxLen = 11; // FAT maximum label length

						labelEffectiveLen = labelLen > labelMaxLen? labelMaxLen : labelLen;

						memset (&NewExtension->wszLabel[labelEffectiveLen], 0, 33 - labelEffectiveLen);
						memcpy (mount->wszLabel, NewExtension->wszLabel, 33);

						__try
						{
							IO_STATUS_BLOCK ioblock;
							ULONG labelInfoSize = sizeof(FILE_FS_LABEL_INFORMATION) + (labelEffectiveLen * sizeof(WCHAR));
							FILE_FS_LABEL_INFORMATION* labelInfo = (FILE_FS_LABEL_INFORMATION*) TCalloc (labelInfoSize);
							if (labelInfo)
							{
								labelInfo->VolumeLabelLength = labelEffectiveLen * sizeof(WCHAR);
								memcpy (labelInfo->VolumeLabel, mount->wszLabel, labelInfo->VolumeLabelLength);

								if (STATUS_SUCCESS == ZwSetVolumeInformationFile (volumeHandle, &ioblock, labelInfo, labelInfoSize, FileFsLabelInformation))
								{
									mount->bDriverSetLabel = TRUE;
									NewExtension->bDriverSetLabel = TRUE;
								}

								TCfree(labelInfo);
							}
						}
						__except (EXCEPTION_EXECUTE_HANDLER)
						{

						}
					}

					TCCloseFsVolume (volumeHandle, volumeFileObject);
				}
			}
			else
			{
				Dump ("Mount FAILURE TC code = 0x%08x\n", mount->nReturnCode);
				TCDeleteDeviceObject (NewDeviceObject, NewExtension);
			}

			return STATUS_SUCCESS;
		}
	}
}
