static VOID SendDeviceIoControlRequestWorkItemRoutine (PDEVICE_OBJECT rootDeviceObject, SendDeviceIoControlRequestWorkItemArgs *arg)
{
	arg->Status = SendDeviceIoControlRequest (arg->deviceObject, arg->ioControlCode, arg->inputBuffer, arg->inputBufferSize, arg->outputBuffer, arg->outputBufferSize);
	KeSetEvent (&arg->WorkItemCompletedEvent, IO_NO_INCREMENT, FALSE);
}
