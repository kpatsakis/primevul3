sf_write_int	(SNDFILE *sndfile, const int *ptr, sf_count_t len)
{	SF_PRIVATE 	*psf ;
	sf_count_t	count ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	if (psf->file.mode == SFM_READ)
	{	psf->error = SFE_NOT_WRITEMODE ;
		return 0 ;
		} ;

	if (len % psf->sf.channels)
	{	psf->error = SFE_BAD_WRITE_ALIGN ;
		return 0 ;
		} ;

	if (psf->write_int == NULL || psf->seek == NULL)
	{	psf->error = SFE_UNIMPLEMENTED ;
		return 0 ;
		} ;

	if (psf->last_op != SFM_WRITE)
		if (psf->seek (psf, SFM_WRITE, psf->write_current) < 0)
			return 0 ;

	if (psf->have_written == SF_FALSE && psf->write_header != NULL)
		psf->write_header (psf, SF_FALSE) ;
	psf->have_written = SF_TRUE ;

	count = psf->write_int (psf, ptr, len) ;

	psf->write_current += count / psf->sf.channels ;

	psf->last_op = SFM_WRITE ;

	if (psf->write_current > psf->sf.frames)
	{	psf->sf.frames = psf->write_current ;
		psf->dataend = 0 ;
		} ;

	if (psf->auto_header && psf->write_header != NULL)
		psf->write_header (psf, SF_TRUE) ;

	return count ;
} /* sf_write_int */
