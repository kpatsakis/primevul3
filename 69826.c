void TCSleep (int milliSeconds)
{
	PKTIMER timer = (PKTIMER) TCalloc (sizeof (KTIMER));
	LARGE_INTEGER duetime;

	if (!timer)
		return;

	duetime.QuadPart = (__int64) milliSeconds * -10000;
	KeInitializeTimerEx(timer, NotificationTimer);
	KeSetTimerEx(timer, duetime, 0, NULL);

	KeWaitForSingleObject (timer, Executive, KernelMode, FALSE, NULL);

	TCfree (timer);
}
