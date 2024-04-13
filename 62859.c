_warc_rduri(const char *buf, size_t bsz)
{
	static const char _key[] = "\r\nWARC-Target-URI:";
	const char *val, *uri, *eol, *p;
	warc_string_t res = {0U, NULL};

	if ((val = xmemmem(buf, bsz, _key, sizeof(_key) - 1U)) == NULL) {
		/* no bother */
		return res;
	}
	/* overread whitespace */
	val += sizeof(_key) - 1U;
	if ((eol = _warc_find_eol(val, buf + bsz - val)) == NULL) {
		/* no end of line */
		return res;
	}

	while (val < eol && (*val == ' ' || *val == '\t'))
		++val;

	/* overread URL designators */
	if ((uri = xmemmem(val, eol - val, "://", 3U)) == NULL) {
		/* not touching that! */
		return res;
	}

	/* spaces inside uri are not allowed, CRLF should follow */
	for (p = val; p < eol; p++) {
		if (isspace((unsigned char)*p))
			return res;
	}

	/* there must be at least space for ftp */
	if (uri < (val + 3U))
		return res;

	/* move uri to point to after :// */
	uri += 3U;

	/* now then, inspect the URI */
	if (memcmp(val, "file", 4U) == 0) {
		/* perfect, nothing left to do here */

	} else if (memcmp(val, "http", 4U) == 0 ||
		   memcmp(val, "ftp", 3U) == 0) {
		/* overread domain, and the first / */
		while (uri < eol && *uri++ != '/');
	} else {
		/* not sure what to do? best to bugger off */
		return res;
	}
	res.str = uri;
	res.len = eol - uri;
	return res;
}
