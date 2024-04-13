NTSTATUS TCCreateRootDeviceObject (PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING Win32NameString, ntUnicodeString;
	WCHAR dosname[32], ntname[32];
	PDEVICE_OBJECT DeviceObject;
	NTSTATUS ntStatus;
	BOOL *bRootExtension;

	Dump ("TCCreateRootDeviceObject BEGIN\n");
	ASSERT (KeGetCurrentIrql() == PASSIVE_LEVEL);

	RtlStringCbCopyW (dosname, sizeof(dosname),(LPWSTR) DOS_ROOT_PREFIX);
	RtlStringCbCopyW (ntname, sizeof(ntname),(LPWSTR) NT_ROOT_PREFIX);
	RtlInitUnicodeString (&ntUnicodeString, ntname);
	RtlInitUnicodeString (&Win32NameString, dosname);

	Dump ("Creating root device nt=%ls dos=%ls\n", ntname, dosname);

	ntStatus = IoCreateDevice (
					  DriverObject,
					  sizeof (BOOL),
					  &ntUnicodeString,
					  FILE_DEVICE_UNKNOWN,
					  FILE_DEVICE_SECURE_OPEN,
					  FALSE,
					  &DeviceObject);

	if (!NT_SUCCESS (ntStatus))
	{
		Dump ("TCCreateRootDeviceObject NTSTATUS = 0x%08x END\n", ntStatus);
		return ntStatus;/* Failed to create DeviceObject */
	}

	DeviceObject->Flags |= DO_DIRECT_IO;
	DeviceObject->AlignmentRequirement = FILE_WORD_ALIGNMENT;

	/* Setup the device extension */
	bRootExtension = (BOOL *) DeviceObject->DeviceExtension;
	*bRootExtension = TRUE;

	KeInitializeMutex (&RootDeviceControlMutex, 0);

	ntStatus = IoCreateSymbolicLink (&Win32NameString, &ntUnicodeString);

	if (!NT_SUCCESS (ntStatus))
	{
		Dump ("TCCreateRootDeviceObject NTSTATUS = 0x%08x END\n", ntStatus);
		IoDeleteDevice (DeviceObject);
		return ntStatus;
	}

	IoRegisterShutdownNotification (DeviceObject);
	RootDeviceObject = DeviceObject;

	Dump ("TCCreateRootDeviceObject STATUS_SUCCESS END\n");
	return STATUS_SUCCESS;
}
