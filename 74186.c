sf_set_string (SNDFILE *sndfile, int str_type, const char* str)
{	SF_PRIVATE 	*psf ;

	VALIDATE_SNDFILE_AND_ASSIGN_PSF (sndfile, psf, 1) ;

	return psf_set_string (psf, str_type, str) ;
} /* sf_get_string */
