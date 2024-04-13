static unsigned int decode_ftime(uint8_t *buf)
{
	int raw;
	struct tm datetime;

	raw = (int) lha_decode_uint32(buf);

	if (raw == 0) {
		return 0;
	}


	memset(&datetime, 0, sizeof(struct tm));

	datetime.tm_sec = (raw << 1) & 0x3e;
	datetime.tm_min = (raw >> 5) & 0x3f;
	datetime.tm_hour = (raw >> 11) & 0x1f;
	datetime.tm_mday = (raw >> 16) & 0x1f;
	datetime.tm_mon = ((raw >> 21) & 0xf) - 1;
	datetime.tm_year = 80 + ((raw >> 25) & 0x7f);
	datetime.tm_wday = 0;
	datetime.tm_yday = 0;
	datetime.tm_isdst = -1;

	return (unsigned int) mktime(&datetime);
}
