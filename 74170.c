sf_error_str (SNDFILE *sndfile, char *str, size_t maxlen)
{	SF_PRIVATE 	*psf ;
	int 		errnum ;

	if (str == NULL)
		return SFE_INTERNAL ;

	if (sndfile == NULL)
		errnum = sf_errno ;
	else
	{	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 0) ;
		errnum = psf->error ;
		} ;

	snprintf (str, maxlen, "%s", sf_error_number (errnum)) ;

	return SFE_NO_ERROR ;
} /* sf_error_str */
