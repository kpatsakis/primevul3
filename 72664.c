static void PrintTime(u64 time)
{
	u32 ms, h, m, s;
	h = (u32) (time / 1000 / 3600);
	m = (u32) (time / 1000 / 60 - h*60);
	s = (u32) (time / 1000 - h*3600 - m*60);
	ms = (u32) (time - (h*3600 + m*60 + s) * 1000);
	fprintf(stderr, "%02d:%02d:%02d.%03d", h, m, s, ms);
}
