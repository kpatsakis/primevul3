vips_foreign_load_gif_header( VipsForeignLoad *load )
{
	VipsObjectClass *class = VIPS_OBJECT_GET_CLASS( load );
	VipsForeignLoadGifClass *gif_class = 
		(VipsForeignLoadGifClass *) VIPS_OBJECT_GET_CLASS( load );
	VipsForeignLoadGif *gif = (VipsForeignLoadGif *) load;

	GifRecordType record;

	if( gif_class->open( gif ) )
		return( -1 );

	gif->n_pages = 0;

	do {
		if( DGifGetRecordType( gif->file, &record ) == GIF_ERROR ) 
			continue;

		switch( record ) {
		case IMAGE_DESC_RECORD_TYPE:
			(void) vips_foreign_load_gif_scan_image( gif );

			gif->n_pages += 1;

			break;

		case EXTENSION_RECORD_TYPE:
			/* We will need to fetch the extensions to check for
			 * cmaps and transparency.
			 */
			(void) vips_foreign_load_gif_scan_extension( gif );
			break;

		case TERMINATE_RECORD_TYPE:
			gif->eof = TRUE;
			break;

		case SCREEN_DESC_RECORD_TYPE:
		case UNDEFINED_RECORD_TYPE:
			break;

		default:
			break;
		}
	} while( !gif->eof );

	if( gif->n == -1 )
		gif->n = gif->n_pages - gif->page;

	if( gif->page < 0 ||
		gif->n <= 0 ||
		gif->page + gif->n > gif->n_pages ) {
		vips_error( class->nickname, "%s", _( "bad page number" ) ); 
		return( -1 ); 
	}

	/* And set the output vips header from what we've learned.
	 */
	if( vips_foreign_load_gif_set_header( gif, load->out ) )
		return( -1 );

	return( 0 );
}
