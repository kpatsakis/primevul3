vips_foreign_load_gif_errstr( int error_code )
{
#ifdef HAVE_GIFLIB_5
	return( GifErrorString( error_code ) ); 
#else /*!HAVE_GIFLIB_5*/
	switch( error_code ) {
	case D_GIF_ERR_OPEN_FAILED:
		return( _( "Failed to open given file" ) ); 

	case D_GIF_ERR_READ_FAILED:
		return( _( "Failed to read from given file" ) ); 

	case D_GIF_ERR_NOT_GIF_FILE:
		return( _( "Data is not a GIF file" ) ); 

	case D_GIF_ERR_NO_SCRN_DSCR:
		return( _( "No screen descriptor detected" ) ); 

	case D_GIF_ERR_NO_IMAG_DSCR:
		return( _( "No image descriptor detected" ) ); 

	case D_GIF_ERR_NO_COLOR_MAP:
		return( _( "Neither global nor local color map" ) ); 

	case D_GIF_ERR_WRONG_RECORD:
		return( _( "Wrong record type detected" ) ); 

	case D_GIF_ERR_DATA_TOO_BIG:
		return( _( "Number of pixels bigger than width * height" ) ); 

	case D_GIF_ERR_NOT_ENOUGH_MEM:
		return( _( "Failed to allocate required memory" ) ); 

	case D_GIF_ERR_CLOSE_FAILED:
		return( _( "Failed to close given file" ) ); 

	case D_GIF_ERR_NOT_READABLE:
		return( _( "Given file was not opened for read" ) ); 

	case D_GIF_ERR_IMAGE_DEFECT:
		return( _( "Image is defective, decoding aborted" ) ); 

	case D_GIF_ERR_EOF_TOO_SOON:
		return( _( "Image EOF detected, before image complete" ) ); 

	default:
		return( _( "Unknown error" ) ); 
	}
#endif /*HAVE_GIFLIB_5*/
}
