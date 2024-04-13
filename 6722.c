static void pgpPrtTime(const char * pre, const uint8_t *p, size_t plen)
{
    if (!_print) return;
    if (pre && *pre)
	fprintf(stderr, "%s", pre);
    if (plen == 4) {
	char buf[1024];
	time_t t = pgpGrab(p, plen);
	struct tm _tm, *tms = localtime_r(&t, &_tm);
	if (strftime(buf, sizeof(buf), "%c", tms) > 0)
	    fprintf(stderr, " %-24.24s(0x%08x)", buf, (unsigned)t);
    } else {
	pgpPrtHex("", p+1, plen-1);
    }
}