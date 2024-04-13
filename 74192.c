sf_write_raw	(SNDFILE *sndfile, const void *ptr, sf_count_t len)
{	SF_PRIVATE 	*psf ;
	sf_count_t	count ;
	int			bytewidth, blockwidth ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	bytewidth = (psf->bytewidth > 0) ? psf->bytewidth : 1 ;
	blockwidth = (psf->blockwidth > 0) ? psf->blockwidth : 1 ;

	if (psf->file.mode == SFM_READ)
	{	psf->error = SFE_NOT_WRITEMODE ;
		return 0 ;
		} ;

	if (len % (psf->sf.channels * bytewidth))
	{	psf->error = SFE_BAD_WRITE_ALIGN ;
		return 0 ;
		} ;

	if (psf->last_op != SFM_WRITE)
		if (psf->seek (psf, SFM_WRITE, psf->write_current) < 0)
			return 0 ;

	if (psf->have_written == SF_FALSE && psf->write_header != NULL)
		psf->write_header (psf, SF_FALSE) ;
	psf->have_written = SF_TRUE ;

	count = psf_fwrite (ptr, 1, len, psf) ;

	psf->write_current += count / blockwidth ;

	psf->last_op = SFM_WRITE ;

	if (psf->write_current > psf->sf.frames)
	{	psf->sf.frames = psf->write_current ;
		psf->dataend = 0 ;
		} ;

	if (psf->auto_header && psf->write_header != NULL)
		psf->write_header (psf, SF_TRUE) ;

	return count ;
} /* sf_write_raw */
