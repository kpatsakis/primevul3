void jas_tmr_stop(jas_tmr_t *tmr)
{
	if (getrusage(RUSAGE_SELF, &tmr->stop) < 0) {
		abort();
	}
}
