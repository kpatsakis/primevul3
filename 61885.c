double jas_tmr_get(jas_tmr_t *tmr)
{
	double t;
	t = ((tmr->stop.ru_utime.tv_sec * 1e6 + tmr->stop.ru_utime.tv_usec) -
	  (tmr->start.ru_utime.tv_sec * 1e6 + tmr->start.ru_utime.tv_usec)) / 1e6;
	t += ((tmr->stop.ru_stime.tv_sec * 1e6 + tmr->stop.ru_stime.tv_usec) -
	  (tmr->start.ru_stime.tv_sec * 1e6 + tmr->start.ru_stime.tv_usec)) / 1e6;
	return t;
}
