copy_filename (SF_PRIVATE *psf, const char *path)
{	const char *ccptr ;
	char *cptr ;

	if (strlen (path) > 1 && strlen (path) - 1 >= sizeof (psf->file.path.c))
	{	psf->error = SFE_FILENAME_TOO_LONG ;
		return psf->error ;
		} ;

	snprintf (psf->file.path.c, sizeof (psf->file.path.c), "%s", path) ;
	if ((ccptr = strrchr (path, '/')) || (ccptr = strrchr (path, '\\')))
		ccptr ++ ;
	else
		ccptr = path ;

	snprintf (psf->file.name.c, sizeof (psf->file.name.c), "%s", ccptr) ;

	/* Now grab the directory. */
	snprintf (psf->file.dir.c, sizeof (psf->file.dir.c), "%s", path) ;
	if ((cptr = strrchr (psf->file.dir.c, '/')) || (cptr = strrchr (psf->file.dir.c, '\\')))
		cptr [1] = 0 ;
	else
		psf->file.dir.c [0] = 0 ;

	return 0 ;
} /* copy_filename */
