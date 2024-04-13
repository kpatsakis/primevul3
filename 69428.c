gstd_error(int pri, int min_stat, const char *s)
{
	char *t1;

	if (gstd_errstring(&t1, min_stat) < 1)
		LOG(pri, ("%s: couldn't form GSSAPI error string", s));
	else {
		LOG(pri, ("%s: %s", s, t1));
		free(t1);
	}
}
