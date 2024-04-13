vips_foreign_load_gif_scan_comment_ext( VipsForeignLoadGif *gif,
	GifByteType *extension )
{
	VIPS_DEBUG_MSG( "gifload: type: comment\n" ); 

	if( !gif->comment ) { 
		/* Up to 257 with a NULL terminator.
		 */
		char comment[257];

		vips_strncpy( comment, (char *) (extension + 1), 256 );
		comment[extension[0]] = '\0';
		gif->comment = g_strdup( comment ); 
	}

	while( extension != NULL ) 
		if( vips_foreign_load_gif_ext_next( gif, &extension ) )
			return( -1 ); 

	return( 0 );
}
