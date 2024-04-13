vips_foreign_load_gif_scan_extension( VipsForeignLoadGif *gif )
{
	GifByteType *extension;
	int ext_code;

	if( DGifGetExtension( gif->file, &ext_code, &extension ) == 
		GIF_ERROR ) {
		vips_foreign_load_gif_error( gif ); 
		return( -1 ); 
	}

	if( extension ) 
		switch( ext_code ) { 
		case GRAPHICS_EXT_FUNC_CODE: 
			if( extension[0] == 4 &&
				extension[1] & 0x1 ) {
				VIPS_DEBUG_MSG( "gifload: has transp.\n" ); 
				gif->has_transparency = TRUE;
			}

			if( !gif->has_delay ) { 
				VIPS_DEBUG_MSG( "gifload: has delay\n" ); 
				gif->has_delay = TRUE;
				gif->delay = extension[2] | (extension[3] << 8);
			}

			while( extension != NULL ) 
				if( vips_foreign_load_gif_ext_next( gif, 
					&extension ) )
					return( -1 ); 

			break;

		case APPLICATION_EXT_FUNC_CODE: 
			if( vips_foreign_load_gif_scan_application_ext( gif, 
				extension ) )
				return( -1 ); 
			break;

		case COMMENT_EXT_FUNC_CODE:
			if( vips_foreign_load_gif_scan_comment_ext( gif, 
				extension ) )
				return( -1 ); 
			break;

		default:
			/* Step over any NEXT blocks for unknown extensions.
			 */
			while( extension != NULL ) 
				if( vips_foreign_load_gif_ext_next( gif, 
					&extension ) )
					return( -1 ); 
			break;
		}

	return( 0 );
}
