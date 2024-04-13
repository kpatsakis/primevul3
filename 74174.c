sf_get_string (SNDFILE *sndfile, int str_type)
{	SF_PRIVATE 	*psf ;

	if ((psf = (SF_PRIVATE*) sndfile) == NULL)
		return NULL ;
	if (psf->Magick != SNDFILE_MAGICK)
		return NULL ;

	return psf_get_string (psf, str_type) ;
} /* sf_get_string */
