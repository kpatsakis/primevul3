VOID VolumeThreadProc (PVOID Context)
{
	PTHREAD_BLOCK pThreadBlock = (PTHREAD_BLOCK) Context;
	PDEVICE_OBJECT DeviceObject = pThreadBlock->DeviceObject;
	PEXTENSION Extension = (PEXTENSION) DeviceObject->DeviceExtension;
	BOOL bDevice;

	/* Set thread priority to lowest realtime level. */
	KeSetPriorityThread (KeGetCurrentThread (), LOW_REALTIME_PRIORITY);

	Dump ("Mount THREAD OPENING VOLUME BEGIN\n");

	if ( !IsDeviceName (pThreadBlock->mount->wszVolume))
	{
		RtlStringCbCopyW (pThreadBlock->wszMountVolume, sizeof(pThreadBlock->wszMountVolume),WIDE ("\\??\\"));
		RtlStringCbCatW (pThreadBlock->wszMountVolume, sizeof(pThreadBlock->wszMountVolume),pThreadBlock->mount->wszVolume);
		bDevice = FALSE;
	}
	else
	{
		pThreadBlock->wszMountVolume[0] = 0;
		RtlStringCbCatW (pThreadBlock->wszMountVolume, sizeof(pThreadBlock->wszMountVolume),pThreadBlock->mount->wszVolume);
		bDevice = TRUE;
	}

	Dump ("Mount THREAD request for File %ls DriveNumber %d Device = %d\n",
	      pThreadBlock->wszMountVolume, pThreadBlock->mount->nDosDriveNo, bDevice);

	pThreadBlock->ntCreateStatus = TCOpenVolume (DeviceObject,
		Extension,
		pThreadBlock->mount,
		pThreadBlock->wszMountVolume,
		bDevice);

	if (!NT_SUCCESS (pThreadBlock->ntCreateStatus) || pThreadBlock->mount->nReturnCode != 0)
	{
		KeSetEvent (&Extension->keCreateEvent, 0, FALSE);
		PsTerminateSystemThread (STATUS_SUCCESS);
	}

	Extension->Queue.IsFilterDevice = FALSE;
	Extension->Queue.DeviceObject = DeviceObject;
	Extension->Queue.CryptoInfo = Extension->cryptoInfo;
	Extension->Queue.HostFileHandle = Extension->hDeviceFile;
	Extension->Queue.VirtualDeviceLength = Extension->DiskLength;
	Extension->Queue.MaxReadAheadOffset.QuadPart = Extension->HostLength;

	if (Extension->SecurityClientContextValid)
		Extension->Queue.SecurityClientContext = &Extension->SecurityClientContext;
	else
		Extension->Queue.SecurityClientContext = NULL;

	pThreadBlock->ntCreateStatus = EncryptedIoQueueStart (&Extension->Queue);

	if (!NT_SUCCESS (pThreadBlock->ntCreateStatus))
	{
		TCCloseVolume (DeviceObject, Extension);

		pThreadBlock->mount->nReturnCode = ERR_OS_ERROR;
		KeSetEvent (&Extension->keCreateEvent, 0, FALSE);
		PsTerminateSystemThread (STATUS_SUCCESS);
	}

	KeSetEvent (&Extension->keCreateEvent, 0, FALSE);
	/* From this point on pThreadBlock cannot be used as it will have been released! */
	pThreadBlock = NULL;

	for (;;)
	{
		/* Wait for a request from the dispatch routines. */
		KeWaitForSingleObject ((PVOID) & Extension->RequestSemaphore, Executive, KernelMode, FALSE, NULL);

		for (;;)
		{
			PIO_STACK_LOCATION irpSp;
			PLIST_ENTRY request;
			PIRP irp;

			request = ExInterlockedRemoveHeadList (&Extension->ListEntry, &Extension->ListSpinLock);
			if (request == NULL)
				break;

			irp = CONTAINING_RECORD (request, IRP, Tail.Overlay.ListEntry);
			irpSp = IoGetCurrentIrpStackLocation (irp);

			ASSERT (irpSp->MajorFunction == IRP_MJ_DEVICE_CONTROL);

			ProcessVolumeDeviceControlIrp (DeviceObject, Extension, irp);
			IoReleaseRemoveLock (&Extension->Queue.RemoveLock, irp);
		}

		if (Extension->bThreadShouldQuit)
		{
			Dump ("Closing volume\n");
			EncryptedIoQueueStop (&Extension->Queue);

			TCCloseVolume (DeviceObject, Extension);
			PsTerminateSystemThread (STATUS_SUCCESS);
		}
	}
}
