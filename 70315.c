is_url(const char *source)
{
	char *colonp;

	/*
	 * RFC 3986 says:
	 *
	 *   URI         = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
	 *
	 *   hier-part   = "//" authority path-abempty
	 *               / path-absolute
	 *               / path-rootless
	 *               / path-empty
	 *
	 *   authority   = [ userinfo "@" ] host [ ":" port ]
	 *
	 *   userinfo    = *( unreserved / pct-encoded / sub-delims / ":" )
	 *
	 * Step 1: look for the ":" at the end of the scheme.
	 * A colon in the source is *NOT* sufficient to indicate that
	 * this is a URL, as interface names on some platforms might
	 * include colons (e.g., I think some Solaris interfaces
	 * might).
	 */
	colonp = strchr(source, ':');
	if (colonp == NULL)
	{
		/*
		 * The source is the device to open.  It's not a URL.
		 */
		return (0);
	}

	/*
	 * All schemes must have "//" after them, i.e. we only support
	 * hier-part   = "//" authority path-abempty, not
	 * hier-part   = path-absolute
	 * hier-part   = path-rootless
	 * hier-part   = path-empty
	 *
	 * We need that in order to distinguish between a local device
	 * name that happens to contain a colon and a URI.
	 */
	if (strncmp(colonp + 1, "//", 2) != 0)
	{
		/*
		 * The source is the device to open.  It's not a URL.
		 */
		return (0);
	}

	/*
	 * It's a URL.
	 */
	return (1);
}
