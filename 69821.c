NTSTATUS TCFsctlCall (PFILE_OBJECT fileObject, LONG IoControlCode,
	void *InputBuffer, int InputBufferSize, void *OutputBuffer, int OutputBufferSize)
{
	IO_STATUS_BLOCK ioStatusBlock;
	NTSTATUS ntStatus;
	PIRP irp;
	KEVENT event;
	PIO_STACK_LOCATION stack;
	PDEVICE_OBJECT deviceObject = IoGetRelatedDeviceObject (fileObject);

	KeInitializeEvent(&event, NotificationEvent, FALSE);

	irp = IoBuildDeviceIoControlRequest (IoControlCode,
					     deviceObject,
					     InputBuffer, InputBufferSize,
					     OutputBuffer, OutputBufferSize,
					     FALSE,
					     &event,
					     &ioStatusBlock);

	if (irp == NULL)
		return STATUS_INSUFFICIENT_RESOURCES;

	stack = IoGetNextIrpStackLocation(irp);

	stack->MajorFunction = IRP_MJ_FILE_SYSTEM_CONTROL;
	stack->MinorFunction = IRP_MN_USER_FS_REQUEST;
	stack->FileObject = fileObject;

	ntStatus = IoCallDriver (deviceObject, irp);
	if (ntStatus == STATUS_PENDING)
	{
		KeWaitForSingleObject (&event, Executive, KernelMode, FALSE, NULL);
		ntStatus = ioStatusBlock.Status;
	}

	return ntStatus;
}
