void sleep_secs(int secs)
{
#ifdef _WIN32
	Sleep(secs*1000);
#else
	unsigned secs_remaining;

	if (secs <= 0)
		return;
	secs_remaining = secs;
	while (secs_remaining != 0)
		secs_remaining = sleep(secs_remaining);
#endif
}
