sf_read_raw		(SNDFILE *sndfile, void *ptr, sf_count_t bytes)
{	SF_PRIVATE 	*psf ;
	sf_count_t	count, extra ;
	int			bytewidth, blockwidth ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	bytewidth = (psf->bytewidth > 0) ? psf->bytewidth : 1 ;
	blockwidth = (psf->blockwidth > 0) ? psf->blockwidth : 1 ;

	if (psf->file.mode == SFM_WRITE)
	{	psf->error = SFE_NOT_READMODE ;
		return	0 ;
		} ;

	if (bytes < 0 || psf->read_current >= psf->sf.frames)
	{	psf_memset (ptr, 0, bytes) ;
		return 0 ;
		} ;

	if (bytes % (psf->sf.channels * bytewidth))
	{	psf->error = SFE_BAD_READ_ALIGN ;
		return 0 ;
		} ;

	if (psf->last_op != SFM_READ)
		if (psf->seek (psf, SFM_READ, psf->read_current) < 0)
			return 0 ;

	count = psf_fread (ptr, 1, bytes, psf) ;

	if (psf->read_current + count / blockwidth <= psf->sf.frames)
		psf->read_current += count / blockwidth ;
	else
	{	count = (psf->sf.frames - psf->read_current) * blockwidth ;
		extra = bytes - count ;
		psf_memset (((char *) ptr) + count, 0, extra) ;
		psf->read_current = psf->sf.frames ;
		} ;

	psf->last_op = SFM_READ ;

	return count ;
} /* sf_read_raw */
