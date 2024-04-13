guess_file_type (SF_PRIVATE *psf)
{	uint32_t buffer [3], format ;

	if (psf_binheader_readf (psf, "b", &buffer, SIGNED_SIZEOF (buffer)) != SIGNED_SIZEOF (buffer))
	{	psf->error = SFE_BAD_FILE_READ ;
		return 0 ;
		} ;

	if ((buffer [0] == MAKE_MARKER ('R', 'I', 'F', 'F') || buffer [0] == MAKE_MARKER ('R', 'I', 'F', 'X'))
			&& buffer [2] == MAKE_MARKER ('W', 'A', 'V', 'E'))
		return SF_FORMAT_WAV ;

	if (buffer [0] == MAKE_MARKER ('F', 'O', 'R', 'M'))
	{	if (buffer [2] == MAKE_MARKER ('A', 'I', 'F', 'F') || buffer [2] == MAKE_MARKER ('A', 'I', 'F', 'C'))
			return SF_FORMAT_AIFF ;
		if (buffer [2] == MAKE_MARKER ('8', 'S', 'V', 'X') || buffer [2] == MAKE_MARKER ('1', '6', 'S', 'V'))
			return SF_FORMAT_SVX ;
		return 0 ;
		} ;

	if (buffer [0] == MAKE_MARKER ('.', 's', 'n', 'd') || buffer [0] == MAKE_MARKER ('d', 'n', 's', '.'))
		return SF_FORMAT_AU ;

	if ((buffer [0] == MAKE_MARKER ('f', 'a', 'p', ' ') || buffer [0] == MAKE_MARKER (' ', 'p', 'a', 'f')))
		return SF_FORMAT_PAF ;

	if (buffer [0] == MAKE_MARKER ('N', 'I', 'S', 'T'))
		return SF_FORMAT_NIST ;

	if (buffer [0] == MAKE_MARKER ('C', 'r', 'e', 'a') && buffer [1] == MAKE_MARKER ('t', 'i', 'v', 'e'))
		return SF_FORMAT_VOC ;

	if ((buffer [0] & MAKE_MARKER (0xFF, 0xFF, 0xF8, 0xFF)) == MAKE_MARKER (0x64, 0xA3, 0x00, 0x00) ||
		(buffer [0] & MAKE_MARKER (0xFF, 0xF8, 0xFF, 0xFF)) == MAKE_MARKER (0x00, 0x00, 0xA3, 0x64))
		return SF_FORMAT_IRCAM ;

	if (buffer [0] == MAKE_MARKER ('r', 'i', 'f', 'f'))
		return SF_FORMAT_W64 ;

	if (buffer [0] == MAKE_MARKER (0, 0, 0x03, 0xE8) && buffer [1] == MAKE_MARKER (0, 0, 0, 1) &&
								buffer [2] == MAKE_MARKER (0, 0, 0, 1))
		return SF_FORMAT_MAT4 ;

	if (buffer [0] == MAKE_MARKER (0, 0, 0, 0) && buffer [1] == MAKE_MARKER (1, 0, 0, 0) &&
								buffer [2] == MAKE_MARKER (1, 0, 0, 0))
		return SF_FORMAT_MAT4 ;

	if (buffer [0] == MAKE_MARKER ('M', 'A', 'T', 'L') && buffer [1] == MAKE_MARKER ('A', 'B', ' ', '5'))
		return SF_FORMAT_MAT5 ;

	if (buffer [0] == MAKE_MARKER ('P', 'V', 'F', '1'))
		return SF_FORMAT_PVF ;

	if (buffer [0] == MAKE_MARKER ('E', 'x', 't', 'e') && buffer [1] == MAKE_MARKER ('n', 'd', 'e', 'd') &&
								buffer [2] == MAKE_MARKER (' ', 'I', 'n', 's'))
		return SF_FORMAT_XI ;

	if (buffer [0] == MAKE_MARKER ('c', 'a', 'f', 'f') && buffer [2] == MAKE_MARKER ('d', 'e', 's', 'c'))
		return SF_FORMAT_CAF ;

	if (buffer [0] == MAKE_MARKER ('O', 'g', 'g', 'S'))
		return SF_FORMAT_OGG ;

	if (buffer [0] == MAKE_MARKER ('A', 'L', 'a', 'w') && buffer [1] == MAKE_MARKER ('S', 'o', 'u', 'n')
			&& buffer [2] == MAKE_MARKER ('d', 'F', 'i', 'l'))
		return SF_FORMAT_WVE ;

	if (buffer [0] == MAKE_MARKER ('D', 'i', 'a', 'm') && buffer [1] == MAKE_MARKER ('o', 'n', 'd', 'W')
			&& buffer [2] == MAKE_MARKER ('a', 'r', 'e', ' '))
		return SF_FORMAT_DWD ;

	if (buffer [0] == MAKE_MARKER ('L', 'M', '8', '9') || buffer [0] == MAKE_MARKER ('5', '3', 0, 0))
		return SF_FORMAT_TXW ;

	if ((buffer [0] & MAKE_MARKER (0xFF, 0xFF, 0x80, 0xFF)) == MAKE_MARKER (0xF0, 0x7E, 0, 0x01))
		return SF_FORMAT_SDS ;

	if ((buffer [0] & MAKE_MARKER (0xFF, 0xFF, 0, 0)) == MAKE_MARKER (1, 4, 0, 0))
		return SF_FORMAT_MPC2K ;

	if (buffer [0] == MAKE_MARKER ('C', 'A', 'T', ' ') && buffer [2] == MAKE_MARKER ('R', 'E', 'X', '2'))
		return SF_FORMAT_REX2 ;

	if (buffer [0] == MAKE_MARKER (0x30, 0x26, 0xB2, 0x75) && buffer [1] == MAKE_MARKER (0x8E, 0x66, 0xCF, 0x11))
		return 0 /*-SF_FORMAT_WMA-*/ ;

	/* HMM (Hidden Markov Model) Tool Kit. */
	if (buffer [2] == MAKE_MARKER (0, 2, 0, 0) && 2 * ((int64_t) BE2H_32 (buffer [0])) + 12 == psf->filelength)
		return SF_FORMAT_HTK ;

	if (buffer [0] == MAKE_MARKER ('f', 'L', 'a', 'C'))
		return SF_FORMAT_FLAC ;

	if (buffer [0] == MAKE_MARKER ('2', 'B', 'I', 'T'))
		return SF_FORMAT_AVR ;

	if (buffer [0] == MAKE_MARKER ('R', 'F', '6', '4') && buffer [2] == MAKE_MARKER ('W', 'A', 'V', 'E'))
		return SF_FORMAT_RF64 ;

	if (buffer [0] == MAKE_MARKER ('I', 'D', '3', 3))
	{	psf_log_printf (psf, "Found 'ID3' marker.\n") ;
		if (id3_skip (psf))
			return guess_file_type (psf) ;
		return 0 ;
		} ;

	/* Turtle Beach SMP 16-bit */
	if (buffer [0] == MAKE_MARKER ('S', 'O', 'U', 'N') && buffer [1] == MAKE_MARKER ('D', ' ', 'S', 'A'))
		return 0 ;

	/* Yamaha sampler format. */
	if (buffer [0] == MAKE_MARKER ('S', 'Y', '8', '0') || buffer [0] == MAKE_MARKER ('S', 'Y', '8', '5'))
		return 0 ;

	if (buffer [0] == MAKE_MARKER ('a', 'j', 'k', 'g'))
		return 0 /*-SF_FORMAT_SHN-*/ ;

	/* This must be the last one. */
	if (psf->filelength > 0 && (format = try_resource_fork (psf)) != 0)
		return format ;

	return 0 ;
} /* guess_file_type */
