evutil_rtrim_lws_(char *str)
{
	char *cp;

	if (str == NULL)
		return;

	if ((cp = strchr(str, '\0')) == NULL || (cp == str))
		return;

	--cp;

	while (*cp == ' ' || *cp == '\t') {
		*cp = '\0';
		if (cp == str)
			break;
		--cp;
	}
}
