vips_foreign_save_new_from_string( const char *string )
{
	const char *file_op;
	GType type;
	VipsForeignSave *save;

	if( !(file_op = vips_foreign_find_save( string )) )
		return( NULL );
	type = g_type_from_name( file_op );
	g_assert( type ); 

	save = VIPS_FOREIGN_SAVE( g_object_new( type, NULL ) );
	g_object_set( save,
		"filename", string,
		NULL );

	return( VIPS_OBJECT( save ) );
}
