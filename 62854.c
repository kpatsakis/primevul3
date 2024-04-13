_warc_find_eol(const char *buf, size_t bsz)
{
	static const char _marker[] = "\r\n";
	const char *hit = xmemmem(buf, bsz, _marker, sizeof(_marker) - 1U);

	return hit;
}
