sf_set_chunk (SNDFILE * sndfile, const SF_CHUNK_INFO * chunk_info)
{	SF_PRIVATE 	*psf ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	if (chunk_info == NULL || chunk_info->data == NULL)
		return SFE_BAD_CHUNK_PTR ;

	if (psf->set_chunk)
		return psf->set_chunk (psf, chunk_info) ;

	return SFE_BAD_CHUNK_FORMAT ;
} /* sf_set_chunk */
