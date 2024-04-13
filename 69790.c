void GetElapsedTimeInit (LARGE_INTEGER *lastPerfCounter)
{
	*lastPerfCounter = KeQueryPerformanceCounter (NULL);
}
