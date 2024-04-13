vips_foreign_is_a( const char *loader, const char *filename )
{
	const VipsObjectClass *class;
	VipsForeignLoadClass *load_class;

	if( !(class = vips_class_find( "VipsForeignLoad", loader )) ) 
		return( FALSE );
	load_class = VIPS_FOREIGN_LOAD_CLASS( class );
	if( load_class->is_a &&
		load_class->is_a( filename ) ) 
		return( TRUE );

	return( FALSE );
}
