sf_next_chunk_iterator (SF_CHUNK_ITERATOR * iterator)
{	SF_PRIVATE 	*psf ;
	SNDFILE	*sndfile = iterator ? iterator->sndfile : NULL ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	if (psf->next_chunk_iterator)
		return psf->next_chunk_iterator (psf, iterator) ;

	return NULL ;
} /* sf_get_chunk_iterator_next */
