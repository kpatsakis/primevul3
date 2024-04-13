double jas_tmr_get(jas_tmr_t *tmr)
{
	double t0;
	double t1;
	t0 = ((double) tmr->start.tv_sec) + ((double) tmr->start.tv_usec) / 1e6;
	t1 = ((double) tmr->stop.tv_sec) + ((double) tmr->stop.tv_usec) / 1e6;
 	return t1 - t0;
 }
