vips_foreign_load_gif_close( VipsForeignLoadGif *gif )
{
#ifdef HAVE_GIFLIB_5
	if( gif->file ) {
		int error; 

		if( DGifCloseFile( gif->file, &error ) == GIF_ERROR ) 
			vips_foreign_load_gif_error_vips( gif, error );
		gif->file = NULL;
	}
#else 
	if( gif->file ) {
		if( DGifCloseFile( gif->file ) == GIF_ERROR ) 
			vips_foreign_load_gif_error_vips( gif, GifLastError() ); 
		gif->file = NULL;
	}
#endif
}
