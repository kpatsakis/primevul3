NTSTATUS TCStartThread (PKSTART_ROUTINE threadProc, PVOID threadArg, PKTHREAD *kThread)
{
	return TCStartThreadInProcess (threadProc, threadArg, kThread, NULL);
}
