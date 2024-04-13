str_matches_option(const char *s1, const char *optionname)
{
	/* Option names are given as "option:" We accept either 'option' in
	 * s1, or 'option:randomjunk'.  The latter form is to implement the
	 * resolv.conf parser. */
	size_t optlen = strlen(optionname);
	size_t slen = strlen(s1);
	if (slen == optlen || slen == optlen - 1)
		return !strncmp(s1, optionname, slen);
	else if (slen > optlen)
		return !strncmp(s1, optionname, optlen);
	else
		return 0;
}
