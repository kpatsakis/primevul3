evutil_getenv_(const char *varname)
{
	if (evutil_issetugid())
		return NULL;

	return getenv(varname);
}
