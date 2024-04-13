sf_current_byterate (SNDFILE *sndfile)
{	SF_PRIVATE 	*psf ;

	if ((psf = (SF_PRIVATE*) sndfile) == NULL)
		return -1 ;
	if (psf->Magick != SNDFILE_MAGICK)
		return -1 ;

	/* This should cover all PCM and floating point formats. */
	if (psf->bytewidth)
		return psf->sf.samplerate * psf->sf.channels * psf->bytewidth ;

	if (psf->byterate)
		return psf->byterate (psf) ;

	switch (SF_CODEC (psf->sf.format))
	{	case SF_FORMAT_IMA_ADPCM :
		case SF_FORMAT_MS_ADPCM :
		case SF_FORMAT_VOX_ADPCM :
			return (psf->sf.samplerate * psf->sf.channels) / 2 ;

		case SF_FORMAT_GSM610 :
			return (psf->sf.samplerate * psf->sf.channels * 13000) / 8000 ;

		case SF_FORMAT_G721_32 :	/* 32kbs G721 ADPCM encoding. */
			return (psf->sf.samplerate * psf->sf.channels) / 2 ;

		case SF_FORMAT_G723_24 :	/* 24kbs G723 ADPCM encoding. */
			return (psf->sf.samplerate * psf->sf.channels * 3) / 8 ;

		case SF_FORMAT_G723_40 :	/* 40kbs G723 ADPCM encoding. */
			return (psf->sf.samplerate * psf->sf.channels * 5) / 8 ;

		default :
			break ;
		} ;

	return -1 ;
} /* sf_current_byterate */
