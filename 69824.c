NTSTATUS TCReadDevice (PDEVICE_OBJECT deviceObject, PVOID buffer, LARGE_INTEGER offset, ULONG length)
{
	return TCReadWriteDevice (FALSE, deviceObject, buffer, offset, length);
}
