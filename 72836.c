rdp_out_unistr(STREAM s, char *string, int len)
{
	/* LEGACY:
	 *
	 *  Do not write new code that uses this function, use the ones defined
	 *  in stream.h for writing utf16 strings to a stream.
	 *
	 */
	static iconv_t icv_local_to_utf16;
	size_t ibl, obl;
	char *pin, *pout;


	if (string == NULL || len == 0)
		return;

	if (!icv_local_to_utf16)
	{
		icv_local_to_utf16 = iconv_open(WINDOWS_CODEPAGE, g_codepage);
		if (icv_local_to_utf16 == (iconv_t) - 1)
		{
			logger(Protocol, Error, "rdo_out_unistr(), iconv_open[%s -> %s] fail %p",
			       g_codepage, WINDOWS_CODEPAGE, icv_local_to_utf16);
			abort();
		}
	}


	ibl = strlen(string);
	obl = len + 2;
	pin = string;
	pout = (char *) s->p;

	memset(pout, 0, len + 4);


	if (iconv(icv_local_to_utf16, (char **) &pin, &ibl, &pout, &obl) == (size_t) - 1)
	{
		logger(Protocol, Error, "rdp_out_unistr(), iconv(2) fail, errno %d", errno);
		abort();
	}

	s->p += len + 2;
}
