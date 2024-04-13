 void jas_tmr_start(jas_tmr_t *tmr)
 {
	if (gettimeofday(&tmr->start, 0)) {
		abort();
	}
}
