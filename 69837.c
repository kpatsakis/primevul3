BOOL ValidateIOBufferSize (PIRP irp, size_t requiredBufferSize, ValidateIOBufferSizeType type)
{
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation (irp);
	BOOL input = (type == ValidateInput || type == ValidateInputOutput);
	BOOL output = (type == ValidateOutput || type == ValidateInputOutput);

	if ((input && irpSp->Parameters.DeviceIoControl.InputBufferLength < requiredBufferSize)
		|| (output && irpSp->Parameters.DeviceIoControl.OutputBufferLength < requiredBufferSize))
	{
		Dump ("STATUS_BUFFER_TOO_SMALL ioctl=0x%x,%d in=%d out=%d reqsize=%d insize=%d outsize=%d\n", (int) (irpSp->Parameters.DeviceIoControl.IoControlCode >> 16), (int) ((irpSp->Parameters.DeviceIoControl.IoControlCode & 0x1FFF) >> 2), input, output, requiredBufferSize, irpSp->Parameters.DeviceIoControl.InputBufferLength, irpSp->Parameters.DeviceIoControl.OutputBufferLength);

		irp->IoStatus.Status = STATUS_BUFFER_TOO_SMALL;
		irp->IoStatus.Information = 0;
		return FALSE;
	}

	if (!input && output)
		memset (irp->AssociatedIrp.SystemBuffer, 0, irpSp->Parameters.DeviceIoControl.OutputBufferLength);

	return TRUE;
}
