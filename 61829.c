static int jas_iccputtime(jas_stream_t *out, jas_icctime_t *time)
{
	jas_iccputuint16(out, time->year);
	jas_iccputuint16(out, time->month);
	jas_iccputuint16(out, time->day);
	jas_iccputuint16(out, time->hour);
	jas_iccputuint16(out, time->min);
	jas_iccputuint16(out, time->sec);
	return 0;
}
