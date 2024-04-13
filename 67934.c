vips_foreign_load_new_from_string( const char *string )
{
	const char *file_op;
	GType type;
	VipsForeignLoad *load;

	if( !(file_op = vips_foreign_find_load( string )) )
		return( NULL );
	type = g_type_from_name( file_op );
	g_assert( type ); 

	load = VIPS_FOREIGN_LOAD( g_object_new( type, NULL ) );
	g_object_set( load,
		"filename", string,
		NULL );

	return( VIPS_OBJECT( load ) );
}
