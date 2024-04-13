static void verbose_stats_dump(int sig _U_)
{
	if (infodelay == 0)
		fprintf(stderr, "Got %u\r", packets_captured);
	alarm(1);
}
