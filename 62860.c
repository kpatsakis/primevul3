_warc_rdver(const char *buf, size_t bsz)
{
	static const char magic[] = "WARC/";
	const char *c;
	unsigned int ver = 0U;
	unsigned int end = 0U;

	if (bsz < 12 || memcmp(buf, magic, sizeof(magic) - 1U) != 0) {
		/* buffer too small or invalid magic */
		return ver;
	}
	/* looks good so far, read the version number for a laugh */
	buf += sizeof(magic) - 1U;

	if (isdigit((unsigned char)buf[0U]) && (buf[1U] == '.') &&
	    isdigit((unsigned char)buf[2U])) {
		/* we support a maximum of 2 digits in the minor version */
		if (isdigit((unsigned char)buf[3U]))
			end = 1U;
		/* set up major version */
		ver = (buf[0U] - '0') * 10000U;
		/* set up minor version */
		if (end == 1U) {
			ver += (buf[2U] - '0') * 1000U;
			ver += (buf[3U] - '0') * 100U;
		} else
			ver += (buf[2U] - '0') * 100U;
		/*
		 * WARC below version 0.12 has a space-separated header
		 * WARC 0.12 and above terminates the version with a CRLF
		 */
		c = buf + 3U + end;
		if (ver >= 1200U) {
			if (memcmp(c, "\r\n", 2U) != 0)
				ver = 0U;
		} else if (ver < 1200U) {
			if (*c != ' ' && *c != '\t')
				ver = 0U;
		}
	}
	return ver;
}
