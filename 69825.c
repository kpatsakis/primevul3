static NTSTATUS TCReadWriteDevice (BOOL write, PDEVICE_OBJECT deviceObject, PVOID buffer, LARGE_INTEGER offset, ULONG length)
{
	NTSTATUS status;
	IO_STATUS_BLOCK ioStatusBlock;
	PIRP irp;
	KEVENT completionEvent;

	ASSERT (KeGetCurrentIrql() <= APC_LEVEL);

	KeInitializeEvent (&completionEvent, NotificationEvent, FALSE);
	irp = IoBuildSynchronousFsdRequest (write ? IRP_MJ_WRITE : IRP_MJ_READ, deviceObject, buffer, length, &offset, &completionEvent, &ioStatusBlock);
	if (!irp)
		return STATUS_INSUFFICIENT_RESOURCES;

	ObReferenceObject (deviceObject);
	status = IoCallDriver (deviceObject, irp);

	if (status == STATUS_PENDING)
	{
		status = KeWaitForSingleObject (&completionEvent, Executive, KernelMode, FALSE, NULL);
		if (NT_SUCCESS (status))
			status = ioStatusBlock.Status;
	}

	ObDereferenceObject (deviceObject);
	return status;
}
