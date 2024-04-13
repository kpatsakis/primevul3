size_t GetCpuCount ()
{
	KAFFINITY activeCpuMap = KeQueryActiveProcessors();
	size_t mapSize = sizeof (activeCpuMap) * 8;
	size_t cpuCount = 0;

	while (mapSize--)
	{
		if (activeCpuMap & 1)
			++cpuCount;

		activeCpuMap >>= 1;
	}

	if (cpuCount == 0)
		return 1;

	return cpuCount;
}
