sf_close	(SNDFILE *sndfile)
{	SF_PRIVATE	*psf ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	return psf_close (psf) ;
} /* sf_close */
