vips_foreign_load_gif_is_a( const char *filename )
{
	unsigned char buf[4];

	if( vips__get_bytes( filename, buf, 4 ) == 4 &&
		vips_foreign_load_gif_is_a_buffer( buf, 4 ) )
		return( 1 );

	return( 0 );
}
