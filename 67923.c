vips_foreign_flags( const char *loader, const char *filename )
{
	const VipsObjectClass *class;

	if( (class = vips_class_find( "VipsForeignLoad", loader )) ) {
		VipsForeignLoadClass *load_class = 
			VIPS_FOREIGN_LOAD_CLASS( class );

		if( load_class->get_flags_filename ) 
			return( load_class->get_flags_filename( filename ) );
	}

	return( 0 );
}
