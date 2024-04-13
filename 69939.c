vips_foreign_load_gif_render( VipsForeignLoadGif *gif ) 
{
	GifFileType *file = gif->file;

	/* Update the colour map for this frame.
	 */
	vips_foreign_load_gif_build_cmap( gif );

	/* BACKGROUND means we reset the frame to 0 (transparent) before we 
	 * render the next set of pixels.
	 */
	if( gif->dispose == DISPOSE_BACKGROUND ) 
		memset( VIPS_IMAGE_ADDR( gif->frame, 0, 0 ), 0, 
			VIPS_IMAGE_SIZEOF_IMAGE( gif->frame ) );

	/* PREVIOUS means we init the frame with the frame before last, ie. we
	 * undo the last render.
	 *
	 * Anything other than PREVIOUS, we must update the previous buffer,
	 */
	if( gif->dispose == DISPOSE_PREVIOUS ) 
		memcpy( VIPS_IMAGE_ADDR( gif->frame, 0, 0 ),
			VIPS_IMAGE_ADDR( gif->previous, 0, 0 ),
			VIPS_IMAGE_SIZEOF_IMAGE( gif->frame ) );
	else 
		memcpy( VIPS_IMAGE_ADDR( gif->previous, 0, 0 ),
			VIPS_IMAGE_ADDR( gif->frame, 0, 0 ),
			VIPS_IMAGE_SIZEOF_IMAGE( gif->frame ) );

	if( file->Image.Interlace ) {
		int i;

		VIPS_DEBUG_MSG( "vips_foreign_load_gif_render: "
			"interlaced frame of %d x %d pixels at %d x %d\n",
			file->Image.Width, file->Image.Height,
			file->Image.Left, file->Image.Top ); 

		for( i = 0; i < 4; i++ ) {
			int y;

			for( y = InterlacedOffset[i]; 
				y < file->Image.Height;
			  	y += InterlacedJumps[i] ) {
				VipsPel *q = VIPS_IMAGE_ADDR( gif->frame, 
					file->Image.Left, file->Image.Top + y );

				if( DGifGetLine( gif->file, gif->line, 
					file->Image.Width ) == GIF_ERROR ) {
					vips_foreign_load_gif_error( gif ); 
					return( -1 ); 
				}

				vips_foreign_load_gif_render_line( gif, 
					file->Image.Width, q, gif->line ); 
			}
		}
	}
	else {
		int y;

		VIPS_DEBUG_MSG( "vips_foreign_load_gif_render: "
			"non-interlaced frame of %d x %d pixels at %d x %d\n",
			file->Image.Width, file->Image.Height,
			file->Image.Left, file->Image.Top ); 

		for( y = 0; y < file->Image.Height; y++ ) {
			VipsPel *q = VIPS_IMAGE_ADDR( gif->frame, 
				file->Image.Left, file->Image.Top + y );

			if( DGifGetLine( gif->file, gif->line, 
				file->Image.Width ) == GIF_ERROR ) {
				vips_foreign_load_gif_error( gif ); 
				return( -1 ); 
			}

			vips_foreign_load_gif_render_line( gif, 
				file->Image.Width, q, gif->line ); 
		}
	}

	return( 0 );
}
