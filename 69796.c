BOOL IsDriveLetterAvailable (int nDosDriveNo, DeviceNamespaceType namespaceType)
{
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING objectName;
	WCHAR link[128];
	HANDLE handle;
	NTSTATUS ntStatus;

	TCGetDosNameFromNumber (link, sizeof(link),nDosDriveNo, namespaceType);
	RtlInitUnicodeString (&objectName, link);
	InitializeObjectAttributes (&objectAttributes, &objectName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	if (NT_SUCCESS (ntStatus = ZwOpenSymbolicLinkObject (&handle, GENERIC_READ, &objectAttributes)))
	{
		ZwClose (handle);
		return FALSE;
	}

	return (ntStatus == STATUS_OBJECT_NAME_NOT_FOUND)? TRUE : FALSE;
}
