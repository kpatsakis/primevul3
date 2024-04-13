static int jas_iccgettime(jas_stream_t *in, jas_icctime_t *time)
{
	if (jas_iccgetuint16(in, &time->year) ||
	  jas_iccgetuint16(in, &time->month) ||
	  jas_iccgetuint16(in, &time->day) ||
	  jas_iccgetuint16(in, &time->hour) ||
	  jas_iccgetuint16(in, &time->min) ||
	  jas_iccgetuint16(in, &time->sec)) {
		return -1;
	}
	return 0;
}
