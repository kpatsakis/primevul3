BOOL IsAccessibleByUser (PUNICODE_STRING objectFileName, BOOL readOnly)
{
	OBJECT_ATTRIBUTES fileObjAttributes;
	IO_STATUS_BLOCK ioStatusBlock;
	HANDLE fileHandle;
	NTSTATUS status;

	ASSERT (!IoIsSystemThread (PsGetCurrentThread()));

	InitializeObjectAttributes (&fileObjAttributes, objectFileName, OBJ_CASE_INSENSITIVE | OBJ_FORCE_ACCESS_CHECK | OBJ_KERNEL_HANDLE, NULL, NULL);

	status = ZwCreateFile (&fileHandle,
		readOnly ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE,
		&fileObjAttributes,
		&ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	if (NT_SUCCESS (status))
	{
		ZwClose (fileHandle);
		return TRUE;
	}

	return FALSE;
}
