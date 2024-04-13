sf_get_chunk_iterator (SNDFILE * sndfile, const SF_CHUNK_INFO * chunk_info)
{	SF_PRIVATE 	*psf ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	if (chunk_info)
		return psf_get_chunk_iterator (psf, chunk_info->id) ;

	return psf_get_chunk_iterator (psf, NULL) ;
} /* sf_get_chunk_iterator */
