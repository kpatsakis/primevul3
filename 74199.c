validate_psf (SF_PRIVATE *psf)
{
	if (psf->datalength < 0)
	{	psf_log_printf (psf, "Invalid SF_PRIVATE field : datalength == %D.\n", psf->datalength) ;
		return 0 ;
		} ;
	if (psf->dataoffset < 0)
	{	psf_log_printf (psf, "Invalid SF_PRIVATE field : dataoffset == %D.\n", psf->dataoffset) ;
		return 0 ;
		} ;
	if (psf->blockwidth && psf->blockwidth != psf->sf.channels * psf->bytewidth)
	{	psf_log_printf (psf, "Invalid SF_PRIVATE field : channels * bytewidth == %d.\n",
								psf->sf.channels * psf->bytewidth) ;
		return 0 ;
		} ;
	return 1 ;
} /* validate_psf */
