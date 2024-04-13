 void jas_tmr_start(jas_tmr_t *tmr)
 {
	if (getrusage(RUSAGE_SELF, &tmr->start) < 0) {
		abort();
	}
}
