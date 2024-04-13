try_resource_fork (SF_PRIVATE * psf)
{	int old_error = psf->error ;

	/* Set READ mode now, to see if resource fork exists. */
	psf->rsrc.mode = SFM_READ ;
	if (psf_open_rsrc (psf) != 0)
	{	psf->error = old_error ;
		return 0 ;
		} ;

	/* More checking here. */
	psf_log_printf (psf, "Resource fork : %s\n", psf->rsrc.path.c) ;

	return SF_FORMAT_SD2 ;
} /* try_resource_fork */
