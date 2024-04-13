static char *tx3g_format_time(u64 ts, u32 timescale, char *szDur, Bool is_srt)
{
	u32 h, m, s, ms;
	ts = (u32) (ts*1000 / timescale);
	h = (u32) (ts / 3600000);
	m = (u32) (ts/ 60000) - h*60;
	s = (u32) (ts/1000) - h*3600 - m*60;
	ms = (u32) (ts) - h*3600000 - m*60000 - s*1000;
	if (is_srt) {
		sprintf(szDur, "%02d:%02d:%02d,%03d", h, m, s, ms);
	} else {
		sprintf(szDur, "%02d:%02d:%02d.%03d", h, m, s, ms);
	}
	return szDur;
}
