evutil_strtoll(const char *s, char **endptr, int base)
{
#ifdef EVENT__HAVE_STRTOLL
	return (ev_int64_t)strtoll(s, endptr, base);
#elif EVENT__SIZEOF_LONG == 8
	return (ev_int64_t)strtol(s, endptr, base);
#elif defined(_WIN32) && defined(_MSC_VER) && _MSC_VER < 1300
	/* XXXX on old versions of MS APIs, we only support base
	 * 10. */
	ev_int64_t r;
	if (base != 10)
		return 0;
	r = (ev_int64_t) _atoi64(s);
	while (isspace(*s))
		++s;
	if (*s == '-')
		++s;
	while (isdigit(*s))
		++s;
	if (endptr)
		*endptr = (char*) s;
	return r;
#elif defined(_WIN32)
	return (ev_int64_t) _strtoi64(s, endptr, base);
#elif defined(EVENT__SIZEOF_LONG_LONG) && EVENT__SIZEOF_LONG_LONG == 8
	long long r;
	int n;
	if (base != 10 && base != 16)
		return 0;
	if (base == 10) {
		n = sscanf(s, "%lld", &r);
	} else {
		unsigned long long ru=0;
		n = sscanf(s, "%llx", &ru);
		if (ru > EV_INT64_MAX)
			return 0;
		r = (long long) ru;
	}
	if (n != 1)
		return 0;
	while (EVUTIL_ISSPACE_(*s))
		++s;
	if (*s == '-')
		++s;
	if (base == 10) {
		while (EVUTIL_ISDIGIT_(*s))
			++s;
	} else {
		while (EVUTIL_ISXDIGIT_(*s))
			++s;
	}
	if (endptr)
		*endptr = (char*) s;
	return r;
#else
#error "I don't know how to parse 64-bit integers."
#endif
}
