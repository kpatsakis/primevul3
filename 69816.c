NTSTATUS TCCompleteIrp (PIRP irp, NTSTATUS status, ULONG_PTR information)
{
	irp->IoStatus.Status = status;
	irp->IoStatus.Information = information;
	IoCompleteRequest (irp, IO_NO_INCREMENT);
	return status;
}
