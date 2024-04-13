vips_foreign_load_gif_open( VipsForeignLoadGif *gif )
{
#ifdef HAVE_GIFLIB_5
{
	int error;

	if( !(gif->file = DGifOpen( gif, gif->read_func, &error )) ) {
		vips_foreign_load_gif_error_vips( gif, error );
		return( -1 ); 
	}
}
#else 
	if( !(gif->file = DGifOpen( gif, gif->read_func )) ) { 
		vips_foreign_load_gif_error_vips( gif, GifLastError() ); 
		return( -1 ); 
	}
#endif

	gif->eof = FALSE;
	gif->current_page = 0;

	/* Allocate a line buffer now that we have the GIF width.
	 */
	VIPS_FREE( gif->line ) 
	if( !(gif->line = VIPS_ARRAY( NULL, gif->file->SWidth, GifPixelType )) )
		return( -1 ); 

	return( 0 );
}
