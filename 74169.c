sf_error_number	(int errnum)
{	static const char *bad_errnum =
		"No error defined for this error number. This is a bug in libsndfile." ;
	int	k ;

	if (errnum == SFE_MAX_ERROR)
		return SndfileErrors [0].str ;

	if (errnum < 0 || errnum > SFE_MAX_ERROR)
	{	/* This really shouldn't happen in release versions. */
		printf ("Not a valid error number (%d).\n", errnum) ;
		return bad_errnum ;
		} ;

	for (k = 0 ; SndfileErrors [k].str ; k++)
		if (errnum == SndfileErrors [k].error)
			return SndfileErrors [k].str ;

	return bad_errnum ;
} /* sf_error_number */
