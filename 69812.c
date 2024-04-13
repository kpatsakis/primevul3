NTSTATUS SendDeviceIoControlRequest (PDEVICE_OBJECT deviceObject, ULONG ioControlCode, void *inputBuffer, int inputBufferSize, void *outputBuffer, int outputBufferSize)
{
	IO_STATUS_BLOCK ioStatusBlock;
	NTSTATUS status;
	PIRP irp;
	KEVENT event;

	if (KeGetCurrentIrql() > APC_LEVEL)
	{
		SendDeviceIoControlRequestWorkItemArgs args;

		PIO_WORKITEM workItem = IoAllocateWorkItem (RootDeviceObject);
		if (!workItem)
			return STATUS_INSUFFICIENT_RESOURCES;

		args.deviceObject = deviceObject;
		args.ioControlCode = ioControlCode;
		args.inputBuffer = inputBuffer;
		args.inputBufferSize = inputBufferSize;
		args.outputBuffer = outputBuffer;
		args.outputBufferSize = outputBufferSize;

		KeInitializeEvent (&args.WorkItemCompletedEvent, SynchronizationEvent, FALSE);
		IoQueueWorkItem (workItem, SendDeviceIoControlRequestWorkItemRoutine, DelayedWorkQueue, &args);

		KeWaitForSingleObject (&args.WorkItemCompletedEvent, Executive, KernelMode, FALSE, NULL);
		IoFreeWorkItem (workItem);

		return args.Status;
	}

	KeInitializeEvent (&event, NotificationEvent, FALSE);

	irp = IoBuildDeviceIoControlRequest (ioControlCode, deviceObject, inputBuffer, inputBufferSize,
		outputBuffer, outputBufferSize, FALSE, &event, &ioStatusBlock);

	if (!irp)
		return STATUS_INSUFFICIENT_RESOURCES;

	ObReferenceObject (deviceObject);

	status = IoCallDriver (deviceObject, irp);
	if (status == STATUS_PENDING)
	{
		KeWaitForSingleObject (&event, Executive, KernelMode, FALSE, NULL);
		status = ioStatusBlock.Status;
	}

	ObDereferenceObject (deviceObject);
	return status;
}
