vips_foreign_load_gif_set_header( VipsForeignLoadGif *gif, VipsImage *image )
{
	vips_image_init_fields( image,
		gif->file->SWidth, gif->file->SHeight * gif->n, 
		(gif->has_colour ? 3 : 1) + (gif->has_transparency ? 1 : 0),
		VIPS_FORMAT_UCHAR, VIPS_CODING_NONE,
		gif->has_colour ? 
		 	VIPS_INTERPRETATION_sRGB : VIPS_INTERPRETATION_B_W,
		1.0, 1.0 );
	vips_image_pipelinev( image, VIPS_DEMAND_STYLE_FATSTRIP, NULL );

	if( vips_object_argument_isset( VIPS_OBJECT( gif ), "n" ) )
		vips_image_set_int( image, 
			VIPS_META_PAGE_HEIGHT, gif->file->SHeight );
	vips_image_set_int( image, VIPS_META_N_PAGES, gif->n_pages );
	vips_image_set_int( image, "gif-delay", gif->delay );
	vips_image_set_int( image, "gif-loop", gif->loop );
	if( gif->comment ) 
		vips_image_set_string( image, "gif-comment", gif->comment );

	return( 0 );
}
