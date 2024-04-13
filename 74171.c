sf_get_chunk_data (const SF_CHUNK_ITERATOR * iterator, SF_CHUNK_INFO * chunk_info)
{	SF_PRIVATE	*psf ;
	SNDFILE	*sndfile = iterator ? iterator->sndfile : NULL ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	if (chunk_info == NULL || chunk_info->data == NULL)
		return SFE_BAD_CHUNK_PTR ;

	if (psf->get_chunk_data)
		return psf->get_chunk_data (psf, iterator, chunk_info) ;

	return SFE_BAD_CHUNK_FORMAT ;
} /* sf_get_chunk_data */
