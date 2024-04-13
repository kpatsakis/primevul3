vips_foreign_load_gif_code_next( VipsForeignLoadGif *gif, 
	GifByteType **extension )
{
	if( DGifGetCodeNext( gif->file, extension ) == GIF_ERROR ) {
		vips_foreign_load_gif_error( gif ); 
		return( -1 ); 
	}

	if( *extension ) 
		VIPS_DEBUG_MSG( "gifload: CODE_NEXT\n" ); 

	return( 0 );
}
