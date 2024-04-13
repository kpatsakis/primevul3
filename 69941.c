vips_foreign_load_gif_scan_application_ext( VipsForeignLoadGif *gif,
	GifByteType *extension )
{
	gboolean have_netscape;

	/* The 11-byte NETSCAPE extension.
	 */
	have_netscape = FALSE;
	if( extension[0] == 11 &&
		vips_isprefix( "NETSCAPE2.0", (const char*) (extension + 1) ) ) 
		have_netscape = TRUE;

	while( extension != NULL ) {
		if( vips_foreign_load_gif_ext_next( gif, &extension ) )
			return( -1 ); 

		if( have_netscape &&
			extension &&
			extension[0] == 3 &&
			extension[1] == 1 ) 
			gif->loop = extension[2] | (extension[3] << 8);
	}

	return( 0 );
}
