DLLIMPORT int cfg_parse_boolean(const char *s)
{
	if (!s) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	if (strcasecmp(s, "true") == 0 || strcasecmp(s, "on") == 0 || strcasecmp(s, "yes") == 0)
		return 1;
	if (strcasecmp(s, "false") == 0 || strcasecmp(s, "off") == 0 || strcasecmp(s, "no") == 0)
		return 0;

	return CFG_FAIL;
}