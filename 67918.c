vips_foreign_find_load_buffer( const void *data, size_t size )
{
	VipsForeignLoadClass *load_class;

	if( !(load_class = (VipsForeignLoadClass *) vips_foreign_map( 
		"VipsForeignLoad",
		(VipsSListMap2Fn) vips_foreign_find_load_buffer_sub, 
		&data, &size )) ) {
		vips_error( "VipsForeignLoad", 
			"%s", _( "buffer is not in a known format" ) ); 
		return( NULL );
	}

	return( G_OBJECT_CLASS_NAME( load_class ) );
}
