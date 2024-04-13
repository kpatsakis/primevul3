NTSTATUS TCOpenFsVolume (PEXTENSION Extension, PHANDLE volumeHandle, PFILE_OBJECT * fileObject)
{
	NTSTATUS ntStatus;
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING fullFileName;
	IO_STATUS_BLOCK ioStatus;
	WCHAR volumeName[TC_MAX_PATH];

	TCGetNTNameFromNumber (volumeName, sizeof(volumeName),Extension->nDosDriveNo);
	RtlInitUnicodeString (&fullFileName, volumeName);
	InitializeObjectAttributes (&objectAttributes, &fullFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	ntStatus = ZwCreateFile (volumeHandle,
		SYNCHRONIZE | GENERIC_READ,
		&objectAttributes,
		&ioStatus,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	Dump ("Volume %ls open NTSTATUS 0x%08x\n", volumeName, ntStatus);

	if (!NT_SUCCESS (ntStatus))
		return ntStatus;

	ntStatus = ObReferenceObjectByHandle (*volumeHandle,
		FILE_READ_DATA,
		NULL,
		KernelMode,
		fileObject,
		NULL);

	if (!NT_SUCCESS (ntStatus))
		ZwClose (*volumeHandle);

	return ntStatus;
}
