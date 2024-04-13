NTSTATUS RemoveDriveLink (int nDosDriveNo)
{
	WCHAR link[256];
	UNICODE_STRING symLink;
	NTSTATUS ntStatus;

	TCGetDosNameFromNumber (link, sizeof(link),nDosDriveNo, DeviceNamespaceDefault);
	RtlInitUnicodeString (&symLink, link);

	ntStatus = IoDeleteSymbolicLink (&symLink);
	Dump ("IoDeleteSymbolicLink returned %X\n", ntStatus);
	return ntStatus;
}
