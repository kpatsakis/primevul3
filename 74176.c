sf_perror (SNDFILE *sndfile)
{	SF_PRIVATE 	*psf ;
	int 		errnum ;

	if (sndfile == NULL)
	{	errnum = sf_errno ;
		}
	else
	{	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 0) ;
		errnum = psf->error ;
		} ;

	fprintf (stderr, "%s\n", sf_error_number (errnum)) ;
	return SFE_NO_ERROR ;
} /* sf_perror */
