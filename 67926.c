vips_foreign_is_a_buffer( const char *loader, const void *data, size_t size )
{
	const VipsObjectClass *class;
	VipsForeignLoadClass *load_class;

	if( !(class = vips_class_find( "VipsForeignLoad", loader )) )
		return( FALSE );
	load_class = VIPS_FOREIGN_LOAD_CLASS( class );
	if( load_class->is_a_buffer &&
		load_class->is_a_buffer( data, size ) )
		return( TRUE );

	return( FALSE );
}
