vips_foreign_find_load_buffer_sub( VipsForeignLoadClass *load_class, 
	const void **buf, size_t *len )
{
	if( load_class->is_a_buffer &&
		load_class->is_a_buffer( *buf, *len ) ) 
		return( load_class );

	return( NULL );
}
