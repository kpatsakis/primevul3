sf_read_double	(SNDFILE *sndfile, double *ptr, sf_count_t len)
{	SF_PRIVATE 	*psf ;
	sf_count_t	count, extra ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	if (psf->file.mode == SFM_WRITE)
	{	psf->error = SFE_NOT_READMODE ;
		return 0 ;
		} ;

	if (len % psf->sf.channels)
	{	psf->error = SFE_BAD_READ_ALIGN ;
		return 0 ;
		} ;

	if (len <= 0 || psf->read_current >= psf->sf.frames)
	{	psf_memset (ptr, 0, len * sizeof (double)) ;
		return 0 ;
		} ;

	if (psf->read_double == NULL || psf->seek == NULL)
	{	psf->error = SFE_UNIMPLEMENTED ;
		return	0 ;
		} ;

	if (psf->last_op != SFM_READ)
		if (psf->seek (psf, SFM_READ, psf->read_current) < 0)
			return 0 ;

	count = psf->read_double (psf, ptr, len) ;

	if (psf->read_current + count / psf->sf.channels <= psf->sf.frames)
		psf->read_current += count / psf->sf.channels ;
	else
	{	count = (psf->sf.frames - psf->read_current) * psf->sf.channels ;
		extra = len - count ;
		psf_memset (ptr + count, 0, extra * sizeof (double)) ;
		psf->read_current = psf->sf.frames ;
		} ;

	psf->last_op = SFM_READ ;

	return count ;
} /* sf_read_double */
