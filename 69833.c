NTSTATUS TCWriteDevice (PDEVICE_OBJECT deviceObject, PVOID buffer, LARGE_INTEGER offset, ULONG length)
{
	return TCReadWriteDevice (TRUE, deviceObject, buffer, offset, length);
}
