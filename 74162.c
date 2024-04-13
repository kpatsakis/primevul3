format_from_extension (SF_PRIVATE *psf)
{	char *cptr ;
	char buffer [16] ;
	int format = 0 ;

	if ((cptr = strrchr (psf->file.name.c, '.')) == NULL)
		return 0 ;

	cptr ++ ;
	if (strlen (cptr) > sizeof (buffer) - 1)
		return 0 ;

	psf_strlcpy (buffer, sizeof (buffer), cptr) ;
	buffer [sizeof (buffer) - 1] = 0 ;

	/* Convert everything in the buffer to lower case. */
	cptr = buffer ;
	while (*cptr)
	{	*cptr = tolower (*cptr) ;
		cptr ++ ;
		} ;

	cptr = buffer ;

	if (strcmp (cptr, "au") == 0)
	{	psf->sf.channels = 1 ;
		psf->sf.samplerate = 8000 ;
		format = SF_FORMAT_RAW | SF_FORMAT_ULAW ;
		}
	else if (strcmp (cptr, "snd") == 0)
	{	psf->sf.channels = 1 ;
		psf->sf.samplerate = 8000 ;
		format = SF_FORMAT_RAW | SF_FORMAT_ULAW ;
		}

	else if (strcmp (cptr, "vox") == 0 || strcmp (cptr, "vox8") == 0)
	{	psf->sf.channels = 1 ;
		psf->sf.samplerate = 8000 ;
		format = SF_FORMAT_RAW | SF_FORMAT_VOX_ADPCM ;
		}
	else if (strcmp (cptr, "vox6") == 0)
	{	psf->sf.channels = 1 ;
		psf->sf.samplerate = 6000 ;
		format = SF_FORMAT_RAW | SF_FORMAT_VOX_ADPCM ;
		}
	else if (strcmp (cptr, "gsm") == 0)
	{	psf->sf.channels = 1 ;
		psf->sf.samplerate = 8000 ;
		format = SF_FORMAT_RAW | SF_FORMAT_GSM610 ;
		}

	/* For RAW files, make sure the dataoffset if set correctly. */
	if ((SF_CONTAINER (format)) == SF_FORMAT_RAW)
		psf->dataoffset = 0 ;

	return format ;
} /* format_from_extension */
