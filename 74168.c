sf_error (SNDFILE *sndfile)
{	SF_PRIVATE	*psf ;

	if (sndfile == NULL)
		return sf_errno ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 0) ;

	if (psf->error)
		return psf->error ;

	return 0 ;
} /* sf_error */
