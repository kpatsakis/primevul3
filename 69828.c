NTSTATUS TCStartThreadInProcess (PKSTART_ROUTINE threadProc, PVOID threadArg, PKTHREAD *kThread, PEPROCESS process)
{
	NTSTATUS status;
	HANDLE threadHandle;
	HANDLE processHandle = NULL;
	OBJECT_ATTRIBUTES threadObjAttributes;

	if (process)
	{
		status = ObOpenObjectByPointer (process, OBJ_KERNEL_HANDLE, NULL, 0, NULL, KernelMode, &processHandle);
		if (!NT_SUCCESS (status))
			return status;
	}

	InitializeObjectAttributes (&threadObjAttributes, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);

	status = PsCreateSystemThread (&threadHandle, THREAD_ALL_ACCESS, &threadObjAttributes, processHandle, NULL, threadProc, threadArg);
	if (!NT_SUCCESS (status))
		return status;

	status = ObReferenceObjectByHandle (threadHandle, THREAD_ALL_ACCESS, NULL, KernelMode, (PVOID *) kThread, NULL);
	if (!NT_SUCCESS (status))
	{
		ZwClose (threadHandle);
		*kThread = NULL;
		return status;
	}

	if (processHandle)
		ZwClose (processHandle);

	ZwClose (threadHandle);
	return STATUS_SUCCESS;
}
