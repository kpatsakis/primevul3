void TCStopThread (PKTHREAD kThread, PKEVENT wakeUpEvent)
{
	if (wakeUpEvent)
		KeSetEvent (wakeUpEvent, 0, FALSE);

	KeWaitForSingleObject (kThread, Executive, KernelMode, FALSE, NULL);
	ObDereferenceObject (kThread);
}
