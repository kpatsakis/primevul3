int64 GetElapsedTime (LARGE_INTEGER *lastPerfCounter)
{
	LARGE_INTEGER freq;
	LARGE_INTEGER counter = KeQueryPerformanceCounter (&freq);

	int64 elapsed = (counter.QuadPart - lastPerfCounter->QuadPart) * 1000000LL / freq.QuadPart;
	*lastPerfCounter = counter;

	return elapsed;
}
