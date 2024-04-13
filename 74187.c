sf_strerror (SNDFILE *sndfile)
{	SF_PRIVATE 	*psf = NULL ;
	int errnum ;

	if (sndfile == NULL)
	{	errnum = sf_errno ;
		if (errnum == SFE_SYSTEM && sf_syserr [0])
			return sf_syserr ;
		}
	else
	{	psf = (SF_PRIVATE *) sndfile ;

		if (psf->Magick != SNDFILE_MAGICK)
			return	"sf_strerror : Bad magic number." ;

		errnum = psf->error ;

		if (errnum == SFE_SYSTEM && psf->syserr [0])
			return psf->syserr ;
		} ;

	return sf_error_number (errnum) ;
} /* sf_strerror */
