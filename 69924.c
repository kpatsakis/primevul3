vips_foreign_load_gif_error( VipsForeignLoadGif *gif )
{
	int error;

	error = 0;

#ifdef HAVE_GIFLIB_5
	if( gif->file ) 
		error = gif->file->Error;
#else 
	error = GifLastError(); 
#endif

	if( error ) 
		vips_foreign_load_gif_error_vips( gif, error ); 
}
