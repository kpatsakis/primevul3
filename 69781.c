NTSTATUS CreateDriveLink (int nDosDriveNo)
{
	WCHAR dev[128], link[128];
	UNICODE_STRING deviceName, symLink;
	NTSTATUS ntStatus;

	TCGetNTNameFromNumber (dev, sizeof(dev),nDosDriveNo);
	TCGetDosNameFromNumber (link, sizeof(link),nDosDriveNo, DeviceNamespaceDefault);

	RtlInitUnicodeString (&deviceName, dev);
	RtlInitUnicodeString (&symLink, link);

	ntStatus = IoCreateSymbolicLink (&symLink, &deviceName);
	Dump ("IoCreateSymbolicLink returned %X\n", ntStatus);
	return ntStatus;
}
