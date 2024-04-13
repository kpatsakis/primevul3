NTSTATUS TCCompleteDiskIrp (PIRP irp, NTSTATUS status, ULONG_PTR information)
{
	irp->IoStatus.Status = status;
	irp->IoStatus.Information = information;
	IoCompleteRequest (irp, NT_SUCCESS (status) ? IO_DISK_INCREMENT : IO_NO_INCREMENT);
	return status;
}
