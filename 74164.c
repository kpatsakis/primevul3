psf_open_file (SF_PRIVATE *psf, SF_INFO *sfinfo)
{	int		error, format ;

	sf_errno = error = 0 ;
	sf_parselog [0] = 0 ;

	if (psf->error)
	{	error = psf->error ;
		goto error_exit ;
		} ;

	if (psf->file.mode != SFM_READ && psf->file.mode != SFM_WRITE && psf->file.mode != SFM_RDWR)
	{	error = SFE_BAD_OPEN_MODE ;
		goto error_exit ;
		} ;

	if (sfinfo == NULL)
	{	error = SFE_BAD_SF_INFO_PTR ;
		goto error_exit ;
		} ;

	if (psf->file.mode == SFM_READ)
	{	if ((SF_CONTAINER (sfinfo->format)) == SF_FORMAT_RAW)
		{	if (sf_format_check (sfinfo) == 0)
			{	error = SFE_RAW_BAD_FORMAT ;
				goto error_exit ;
				} ;
			}
		else
			memset (sfinfo, 0, sizeof (SF_INFO)) ;
		} ;

	memcpy (&psf->sf, sfinfo, sizeof (SF_INFO)) ;

	psf->Magick 		= SNDFILE_MAGICK ;
	psf->norm_float 	= SF_TRUE ;
	psf->norm_double	= SF_TRUE ;
	psf->dataoffset		= -1 ;
	psf->datalength		= -1 ;
	psf->read_current	= -1 ;
	psf->write_current	= -1 ;
	psf->auto_header 	= SF_FALSE ;
	psf->rwf_endian		= SF_ENDIAN_LITTLE ;
	psf->seek			= psf_default_seek ;
	psf->float_int_mult = 0 ;
	psf->float_max		= -1.0 ;

	/* An attempt at a per SF_PRIVATE unique id. */
	psf->unique_id		= psf_rand_int32 () ;

	psf->sf.sections = 1 ;

	psf->is_pipe = psf_is_pipe (psf) ;

	if (psf->is_pipe)
	{	psf->sf.seekable = SF_FALSE ;
		psf->filelength = SF_COUNT_MAX ;
		}
	else
	{	psf->sf.seekable = SF_TRUE ;

		/* File is open, so get the length. */
		psf->filelength = psf_get_filelen (psf) ;
		} ;

	if (psf->fileoffset > 0)
	{	switch (psf->file.mode)
		{	case SFM_READ :
				if (psf->filelength < 44)
				{	psf_log_printf (psf, "Short filelength: %D (fileoffset: %D)\n", psf->filelength, psf->fileoffset) ;
					error = SFE_BAD_OFFSET ;
					goto error_exit ;
					} ;
				break ;

			case SFM_WRITE :
				psf->fileoffset = 0 ;
				psf_fseek (psf, 0, SEEK_END) ;
				psf->fileoffset = psf_ftell (psf) ;
				break ;

			case SFM_RDWR :
				error = SFE_NO_EMBEDDED_RDWR ;
				goto error_exit ;
			} ;

		psf_log_printf (psf, "Embedded file offset : %D\n", psf->fileoffset) ;
		} ;

	if (psf->filelength == SF_COUNT_MAX)
		psf_log_printf (psf, "Length : unknown\n") ;
	else
		psf_log_printf (psf, "Length : %D\n", psf->filelength) ;

	if (psf->file.mode == SFM_WRITE || (psf->file.mode == SFM_RDWR && psf->filelength == 0))
	{	/* If the file is being opened for write or RDWR and the file is currently
		** empty, then the SF_INFO struct must contain valid data.
		*/
		if ((SF_CONTAINER (psf->sf.format)) == 0)
		{	error = SFE_ZERO_MAJOR_FORMAT ;
			goto error_exit ;
			} ;
		if ((SF_CODEC (psf->sf.format)) == 0)
		{	error = SFE_ZERO_MINOR_FORMAT ;
			goto error_exit ;
			} ;

		if (sf_format_check (&psf->sf) == 0)
		{	error = SFE_BAD_OPEN_FORMAT ;
			goto error_exit ;
			} ;
		}
	else if ((SF_CONTAINER (psf->sf.format)) != SF_FORMAT_RAW)
	{	/* If type RAW has not been specified then need to figure out file type. */
		psf->sf.format = guess_file_type (psf) ;

		if (psf->sf.format == 0)
			psf->sf.format = format_from_extension (psf) ;
		} ;

	/* Prevent unnecessary seeks */
	psf->last_op = psf->file.mode ;

	/* Set bytewidth if known. */
	switch (SF_CODEC (psf->sf.format))
	{	case SF_FORMAT_PCM_S8 :
		case SF_FORMAT_PCM_U8 :
		case SF_FORMAT_ULAW :
		case SF_FORMAT_ALAW :
		case SF_FORMAT_DPCM_8 :
				psf->bytewidth = 1 ;
				break ;

		case SF_FORMAT_PCM_16 :
		case SF_FORMAT_DPCM_16 :
				psf->bytewidth = 2 ;
				break ;

		case SF_FORMAT_PCM_24 :
				psf->bytewidth = 3 ;
				break ;

		case SF_FORMAT_PCM_32 :
		case SF_FORMAT_FLOAT :
				psf->bytewidth = 4 ;
				break ;

		case SF_FORMAT_DOUBLE :
				psf->bytewidth = 8 ;
				break ;
		} ;

	/* Call the initialisation function for the relevant file type. */
	switch (SF_CONTAINER (psf->sf.format))
	{	case	SF_FORMAT_WAV :
		case	SF_FORMAT_WAVEX :
				error = wav_open (psf) ;
				break ;

		case	SF_FORMAT_AIFF :
				error = aiff_open (psf) ;
				break ;

		case	SF_FORMAT_AU :
				error = au_open (psf) ;
				break ;

		case	SF_FORMAT_RAW :
				error = raw_open (psf) ;
				break ;

		case	SF_FORMAT_W64 :
				error = w64_open (psf) ;
				break ;

		case	SF_FORMAT_RF64 :
				error = rf64_open (psf) ;
				break ;

		/* Lite remove start */
		case	SF_FORMAT_PAF :
				error = paf_open (psf) ;
				break ;

		case	SF_FORMAT_SVX :
				error = svx_open (psf) ;
				break ;

		case	SF_FORMAT_NIST :
				error = nist_open (psf) ;
				break ;

		case	SF_FORMAT_IRCAM :
				error = ircam_open (psf) ;
				break ;

		case	SF_FORMAT_VOC :
				error = voc_open (psf) ;
				break ;

		case	SF_FORMAT_SDS :
				error = sds_open (psf) ;
				break ;

		case	SF_FORMAT_OGG :
				error = ogg_open (psf) ;
				break ;

		case	SF_FORMAT_TXW :
				error = txw_open (psf) ;
				break ;

		case	SF_FORMAT_WVE :
				error = wve_open (psf) ;
				break ;

		case	SF_FORMAT_DWD :
				error = dwd_open (psf) ;
				break ;

		case	SF_FORMAT_MAT4 :
				error = mat4_open (psf) ;
				break ;

		case	SF_FORMAT_MAT5 :
				error = mat5_open (psf) ;
				break ;

		case	SF_FORMAT_PVF :
				error = pvf_open (psf) ;
				break ;

		case	SF_FORMAT_XI :
				error = xi_open (psf) ;
				break ;

		case	SF_FORMAT_HTK :
				error = htk_open (psf) ;
				break ;

		case	SF_FORMAT_SD2 :
				error = sd2_open (psf) ;
				break ;

		case	SF_FORMAT_REX2 :
				error = rx2_open (psf) ;
				break ;

		case	SF_FORMAT_AVR :
				error = avr_open (psf) ;
				break ;

		case	SF_FORMAT_FLAC :
				error = flac_open (psf) ;
				break ;

		case	SF_FORMAT_CAF :
				error = caf_open (psf) ;
				break ;

		case	SF_FORMAT_MPC2K :
				error = mpc2k_open (psf) ;
				break ;

		/* Lite remove end */

		default :
				error = SFE_UNKNOWN_FORMAT ;
		} ;

	if (error)
		goto error_exit ;

	/* For now, check whether embedding is supported. */
	format = SF_CONTAINER (psf->sf.format) ;
	if (psf->fileoffset > 0)
	{	switch (format)
		{	case SF_FORMAT_WAV :
			case SF_FORMAT_WAVEX :
			case SF_FORMAT_AIFF :
			case SF_FORMAT_AU :
				/* Actual embedded files. */
				break ;

			case SF_FORMAT_FLAC :
				/* Flac with an ID3v2 header? */
				break ;

			default :
				error = SFE_NO_EMBED_SUPPORT ;
				goto error_exit ;
			} ;
		} ;

	if (psf->fileoffset > 0)
		psf_log_printf (psf, "Embedded file length : %D\n", psf->filelength) ;

	if (psf->file.mode == SFM_RDWR && sf_format_check (&psf->sf) == 0)
	{	error = SFE_BAD_MODE_RW ;
		goto error_exit ;
		} ;

	if (validate_sfinfo (&psf->sf) == 0)
	{	psf_log_SF_INFO (psf) ;
		save_header_info (psf) ;
		error = SFE_BAD_SF_INFO ;
		goto error_exit ;
		} ;

	if (validate_psf (psf) == 0)
	{	save_header_info (psf) ;
		error = SFE_INTERNAL ;
		goto error_exit ;
		} ;

	psf->read_current = 0 ;
	psf->write_current = 0 ;
	if (psf->file.mode == SFM_RDWR)
	{	psf->write_current = psf->sf.frames ;
		psf->have_written = psf->sf.frames > 0 ? SF_TRUE : SF_FALSE ;
		} ;

	memcpy (sfinfo, &psf->sf, sizeof (SF_INFO)) ;

	if (psf->file.mode == SFM_WRITE)
	{	/* Zero out these fields. */
		sfinfo->frames = 0 ;
		sfinfo->sections = 0 ;
		sfinfo->seekable = 0 ;
		} ;

	return (SNDFILE *) psf ;

error_exit :
	sf_errno = error ;

	if (error == SFE_SYSTEM)
		snprintf (sf_syserr, sizeof (sf_syserr), "%s", psf->syserr) ;
	snprintf (sf_parselog, sizeof (sf_parselog), "%s", psf->parselog.buf) ;

	switch (error)
	{	case SF_ERR_SYSTEM :
		case SF_ERR_UNSUPPORTED_ENCODING :
		case SFE_UNIMPLEMENTED :
			break ;

		case SFE_RAW_BAD_FORMAT :
			break ;

		default :
			if (psf->file.mode == SFM_READ)
			{	psf_log_printf (psf, "Parse error : %s\n", sf_error_number (error)) ;
				error = SF_ERR_MALFORMED_FILE ;
				} ;
		} ;

	psf_close (psf) ;
	return NULL ;
} /* psf_open_file */
