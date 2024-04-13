_warc_find_eoh(const char *buf, size_t bsz)
{
	static const char _marker[] = "\r\n\r\n";
	const char *hit = xmemmem(buf, bsz, _marker, sizeof(_marker) - 1U);

	if (hit != NULL) {
		hit += sizeof(_marker) - 1U;
	}
	return hit;
}
