vips_foreign_load_gif_next_page( VipsForeignLoadGif *gif )
{
	GifRecordType record;
	gboolean have_read_frame;

	have_read_frame = FALSE;
	do { 
		if( DGifGetRecordType( gif->file, &record ) == GIF_ERROR ) {
			vips_foreign_load_gif_error( gif ); 
			return( -1 ); 
		}

		switch( record ) {
		case IMAGE_DESC_RECORD_TYPE:
			VIPS_DEBUG_MSG( "vips_foreign_load_gif_next_page: "
				"IMAGE_DESC_RECORD_TYPE\n" ); 

			if( DGifGetImageDesc( gif->file ) == GIF_ERROR ) {
				vips_foreign_load_gif_error( gif ); 
				return( -1 ); 
			}

			if( vips_foreign_load_gif_render( gif ) )
				return( -1 ); 

			have_read_frame = TRUE;

			break;

		case EXTENSION_RECORD_TYPE:
			if( vips_foreign_load_gif_extension( gif ) )
				return( -1 );
			break;

		case TERMINATE_RECORD_TYPE:
			VIPS_DEBUG_MSG( "vips_foreign_load_gif_next_page: "
				"TERMINATE_RECORD_TYPE\n" ); 
			gif->eof = TRUE;
			break;

		case SCREEN_DESC_RECORD_TYPE:
			VIPS_DEBUG_MSG( "vips_foreign_load_gif_next_page: "
				"SCREEN_DESC_RECORD_TYPE\n" );
			break;

		case UNDEFINED_RECORD_TYPE:
			VIPS_DEBUG_MSG( "vips_foreign_load_gif_next_page: "
				"UNDEFINED_RECORD_TYPE\n" );
			break;

		default:
			break;
		}
	} while( !have_read_frame &&
		!gif->eof );

	return( 0 );
}
