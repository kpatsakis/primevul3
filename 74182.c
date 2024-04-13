sf_readf_float	(SNDFILE *sndfile, float *ptr, sf_count_t frames)
{	SF_PRIVATE 	*psf ;
	sf_count_t	count, extra ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	if (psf->file.mode == SFM_WRITE)
	{	psf->error = SFE_NOT_READMODE ;
		return 0 ;
		} ;

	if (frames <= 0 || psf->read_current >= psf->sf.frames)
	{	psf_memset (ptr, 0, frames * psf->sf.channels * sizeof (float)) ;
		return 0 ;
		} ;

	if (psf->read_float == NULL || psf->seek == NULL)
	{	psf->error = SFE_UNIMPLEMENTED ;
		return	0 ;
		} ;

	if (psf->last_op != SFM_READ)
		if (psf->seek (psf, SFM_READ, psf->read_current) < 0)
			return 0 ;

	count = psf->read_float (psf, ptr, frames * psf->sf.channels) ;

	if (psf->read_current + count / psf->sf.channels <= psf->sf.frames)
		psf->read_current += count / psf->sf.channels ;
	else
	{	count = (psf->sf.frames - psf->read_current) * psf->sf.channels ;
		extra = frames * psf->sf.channels - count ;
		psf_memset (ptr + count, 0, extra * sizeof (float)) ;
		psf->read_current = psf->sf.frames ;
		} ;

	psf->last_op = SFM_READ ;

	return count / psf->sf.channels ;
} /* sf_readf_float */
