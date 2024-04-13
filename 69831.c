void TCStopVolumeThread (PDEVICE_OBJECT DeviceObject, PEXTENSION Extension)
{
	NTSTATUS ntStatus;

	UNREFERENCED_PARAMETER (DeviceObject);	/* Remove compiler warning */

	Dump ("Signalling thread to quit...\n");

	Extension->bThreadShouldQuit = TRUE;

	KeReleaseSemaphore (&Extension->RequestSemaphore,
			    0,
			    1,
			    TRUE);

	ntStatus = KeWaitForSingleObject (Extension->peThread,
					  Executive,
					  KernelMode,
					  FALSE,
					  NULL);

	ASSERT (NT_SUCCESS (ntStatus));

	ObDereferenceObject (Extension->peThread);
	Extension->peThread = NULL;

	Dump ("Thread exited\n");
}
